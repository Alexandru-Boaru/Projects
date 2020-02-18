module ClassState
where

import qualified Data.Map as Map


-- Utilizat pentru a obține informații despre Variabile sau Funcții
data InstrType = Var | Func  deriving (Show, Eq)

-- Definirea unor tipuri ce vor fi folosite de ClassState
type TipVar = String                        -- tipul variabilei
type SimbVar = String                       -- simbolul variabilei
type TipRet = String                        -- tipul returnat de functie
type TipParam = String                      -- un parametru al unei functii
type SimbFunc = String                      -- simbolul unei functii
type VarSec = Map.Map SimbVar TipVar        -- map-ul pentru variabile
type FuncSec = Map.Map SimbFunc [[String]]  -- map-ul pentru functii
-- ClassState are un singur constructor cu doi parametrii:
-- un map pentru variabile si un map pentru functii
data ClassState = ClassState VarSec FuncSec deriving (Show, Eq)


-- functia initializeaza un ClassState cu map-uri goale pentru variabile si pentru functii
initEmptyClass :: ClassState
initEmptyClass = ClassState Map.empty Map.empty

-- functia verifica tipul parametrului InstrType si in functie de el se apeleaza o functie auxiliara
insertIntoClass :: ClassState -> InstrType -> [String] -> ClassState
insertIntoClass c t s
  | t == Var = insertIntoVar c s
  | t == Func = insertIntoFunc c s

-- functie auxiliara pentru inserare de variabile
insertIntoVar :: ClassState -> [String] -> ClassState
insertIntoVar (ClassState v f) [simb_var,tip_var] = (ClassState (Map.insert simb_var tip_var v) f)

-- functie auxiliara pentru inserare de functii
insertIntoFunc :: ClassState -> [String] -> ClassState
insertIntoFunc (ClassState v f) (simb_func:tip_ret:l) =
  if(Map.notMember simb_func f)
    then (ClassState v (Map.insert simb_func [simb_func:tip_ret:l] f))
    else (ClassState v (Map.adjust (\x-> (simb_func:tip_ret:l):x) simb_func f) )

-- functia verifica tipul parametrului InstrType si in functie de el se apeleaza o functie auxiliara
getValues :: ClassState -> InstrType -> [[String]]
getValues (ClassState v f) t
  |t == Var = getValuesVar v
  |t == Func = getValuesFunc f
-- functie auxiliara care intoarce varibilele din ClassState
getValuesVar :: Map.Map String String -> [[String]]
getValuesVar m = if(Map.null m) then [] else foldr (\x y -> [x,(m Map.! x)]:y) [] (Map.keys m)

-- functie auxiliara care intoarce functiile din ClassState
getValuesFunc :: Map.Map String [[String]] -> [[String]]
getValuesFunc m = if(Map.null m) then [] else foldr (\x y -> foldr (\a b-> a:b) y (m Map.! x)) [] (Map.keys m)
