module Parser
where

import Util
import Data.Maybe
import InferenceDataType
import ClassState
import qualified Data.Map as Map
import qualified Data.List as List
-- Definire Program
-- Definirea unor tipuri ce vor fi folosite de Program
type ClassName = String   --nume clasa
type ParentName = String  --nume clasa parinte
type VarName = String     --nume variabila
type FuncName = String    --nume functie
type ExprName = String    --nume expresie
--clasa este un map care chei de tip ClassName si valori de tip perechi
--de nume clasa parinte si ClassState
type Class = Map.Map ClassName (ParentName,ClassState)
-- Program este definit ca un map care contine toate clasele
data Program = Program Class deriving (Show, Eq)

-- functia de initializare intoarce un Program care are o singura clasa: Global
initEmptyProgram :: Program
initEmptyProgram = Program (Map.singleton "Global" ("Global",initEmptyClass))

-- functia care intoarce toate variabilele din Program
getVars :: Program -> [[String]]
getVars (Program p) = getValues (snd (p Map.! "Global")) Var

-- functie care intoarce toate clasele existente in Program
getClasses :: Program -> [String]
getClasses (Program p) = Map.keys p

-- functie care intoarce clasa parinte unei clase date ca parametru (daca aceasta exista)
getParentClass :: String -> Program -> String
getParentClass s (Program p) = if(Map.member s p) then fst (p Map.! s) else ""

-- functie care intoarce toate functiile unei clase prezente in Program
getFuncsForClass :: String -> Program -> [[String]]
getFuncsForClass s (Program p) = if(Map.member s p) then getValues (snd (p Map.! s)) Func else []

-- Instruction poate fi ce considerați voi

-- Instruction este definit prin constructori pentru toate tipurile de instructiuni posibile
  -- ClassDeclare: instructiune pentru clase
  -- VarDeclare: instructiune pentru variabile
  -- FuncDeclare: instructiune pentru functii
  -- ExprDeclare: instructiune pentru expresii (din BONUS)
data Instruction = ClassDeclare ClassName ParentName
                   |VarDeclare VarName TipRet
                   |FuncDeclare ClassName FuncName TipRet [TipParam]
                   |ExprDeclare ExprName [String]
                   deriving (Show,Eq)

-- functie care imparte string-ul s in functie de niste delimitatori d
split' :: String -> String-> [String]
split' d s = if(null s) then [] else x: split' d (drop 1 y) where(x,y)=span (\x->notElem x d) s

-- functie care imparte string-ul s in linii si apoi se apeleaza split' pe fiecare linie
split'' :: String -> [[String]]
split'' s = foldr (\x y -> if((take 5 x)/="infer")
                            then ((filter(/="") (split' " ,:;()=" x)):y)
                            else ([x]:y) )
                            [] (filter (/="") (lines s))

-- functia de parsare aplica makeInstruction pe fiecare linie primita din input
parse :: String -> [Instruction]
parse s = foldr(\x y-> (makeInstruction x):y) [] (split'' s)

-- functia transforma o linie de input in tipul de instructiune necesar
makeInstruction :: [String] -> Instruction
makeInstruction s
  |head s == "class" = if (length s==2)
                        then ClassDeclare (head$tail s) "Global"
                        else ClassDeclare (head$tail s) (last s)
  |head s == "newvar" = VarDeclare (head$tail s) (last s)
  |(take 5 (head s)) == "infer" = (\x-> (ExprDeclare (head x) (specialParse (last x)) ) )
                                  (split' "=" (filter (/=' ') (drop 5 (head s))))
  |otherwise = FuncDeclare (head$tail s) (head$tail$tail s) (head s)(tail$tail$tail s)

-- o functie de parsare speciala folosita numai de instructiunea infer
specialParse :: String -> [String]
specialParse s = filter (/=",") ((\(a,b)-> foldl(\acc x -> x:acc) [] a)
                (foldl(\(y,z) x-> (\(a,b)->((drop z a):y,x))
                (splitAt x fexpr)) ([],0) (List.sort lt)) )
  where lt=List.union (map (+1) l) l
        l=List.findIndices (\x-> elem x ",.()") fexpr
        fexpr = filter (/=' ') s

-- functie care intoarce numarul de elemente (token-uri) din constructia unei functii intr-o expresie
takeAux :: [String]->Int->Int->Int
takeAux l a acc
  |a==0 && acc>=4 = acc
  |null l= acc
  |head l== "(" = takeAux (tail l) (a+1) (acc+1)
  |head l== ")" = takeAux (tail l) (a-1) (acc+1)
  |otherwise = takeAux (tail l) a (acc+1)

-- functie care converteste o instructiune primita ca lista de string-uri intr-o lista de expresii
instrToExpr :: [String] -> [Expr] -> [Expr]
instrToExpr s acc
  |null s = acc
  |head s==")" = acc
  |(s !! 1)=="." && (s!!3)=="(" = instrToExpr (drop (takeAux s 0 0) s)
                                  (acc++[FCall (head s) (s!!2) (instrToExpr (drop 4 s) [] )])
  |otherwise = instrToExpr (tail s) (acc++[Va(head s)])

-- functie care interpreteaza instructiunile si introduce (daca se poate)
-- o varibila/functie noua in program
interpret :: Instruction -> Program -> Program
-- se introduce o clasa noua
interpret (ClassDeclare cn pn) (Program p) =
  if(elem pn (getClasses (Program p)))
    then Program (Map.insert cn (pn,initEmptyClass) p)
    else Program (Map.insert cn ("Global",initEmptyClass) p)

-- se introduce o variabila noua
interpret (VarDeclare vn tr) (Program p) =
  if(elem tr (getClasses (Program p)))
    then Program (Map.adjust (\(x,y) -> (x,insertIntoClass y Var [vn,tr])) "Global" p )
    else Program p

-- se introduce o functie noua intr-o clasa cn
interpret (FuncDeclare cn fn tr tp) (Program p) =
  if(elem cn (getClasses (Program p)) &&
    elem tr (getClasses (Program p)) &&
    (List.intersect tp (getClasses (Program p)))==tp)

    then Program (Map.adjust (\(x,y) -> (x,insertIntoClass y Func (fn:tr:tp))) cn p )
    else Program p

-- se introduce o variabila noua creata in urma unei expresii
interpret (ExprDeclare en s) (Program p) =
  if(expr/=Nothing)
    then Program (Map.adjust (\(x,y) -> (x,insertIntoClass y Var [en,tip])) "Global" p )
    else Program p
  where expr = infer (head (instrToExpr s [])) (Program p)
        tip = if(expr == Nothing ) then "" else ((\(Just x)-> x) expr)

-- functie care intoarce numele tuturor variabilelor
getVarsNames :: Program -> [String]
getVarsNames p = map (head) (getVars p)

-- functie care intoarce tipul unei varibile
getVarType :: Program -> String -> String
getVarType p s = (head.tail.head) (filter (\x -> head x==s) (getVars p))

-- functie care cauta tipul de return al unei functii cu numele sf si parametrii tp,
-- plecand din clasa c si urcand pe lantul mostenirilor
searchForType :: Program -> String -> [String] -> String -> Maybe String
searchForType p sf tp c=
  if(length l == 0)
    then
      if(c/=getParentClass c p)
        then searchForType p sf tp (getParentClass c p)
        else Nothing
    else Just (head l)
    where l = map (\x -> head(tail x))
              (filter(\x -> head x==sf && tail(tail x)==tp) (getFuncsForClass c p))

-- functia de inferenta a unei expresii
infer :: Expr -> Program -> Maybe String
infer (Va s) p = if(elem s (getVarsNames p)) then (Just (getVarType p s)) else Nothing
infer (FCall sv sf tp) p=
  if(elem sv (getVarsNames p))
    then
      if(notElem Nothing tj)
        then
          if((searchForType p sf (map (\(Just x)->x) tj) (getVarType p sv))/=Nothing)
            then searchForType p sf (map (\(Just x)->x) tj) (getVarType p sv)
            else Nothing
        else Nothing
    else Nothing
    where tj = map (\x -> infer x p) tp
