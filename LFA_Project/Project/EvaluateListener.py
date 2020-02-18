import regex as rx
from regex import RegEx

from RegexListener import RegexListener

#intoarce token-ul sub forma de string
def toString(obj):
	return str(obj)

#intoarce token-ul sub forma de integer
def toInteger(obj):
	return int(toString(obj));

#verifica un element anterior din stiva de instructiuni
def instr(l, pos):
	if(len(l) < pos):
		return None;
	return l[len(l)-pos]

#intoarce numarul din interiorul token-ului R_FIXED
def getNumber(obj):
	num = str(obj)
	return int(num[1:-1])

#intoarce primul numar din interiorul unui token de tip range
def getFirstNumber(obj):
	num = str(obj)
	return int(num[1:num.index(",")])

#intoarce ultimul numar din interiorul unui token de tip range
def getLastNumber(obj):
	num = str(obj)
	return int(num[num.index(",")+1: -1])

class EvaluateListener(RegexListener):

	def __init__(self):
		self.stack = []
		self.set = set()

	#functie care ia elemente din stiva, si le transforma in obiecte de tip 
	#RegEx. La final in stiva se va afla RegEx-ul expresiei primite la input
	def clean(self, ex):

		if instr(self.stack, 1) == "*":
			self.stack.pop()
			self.clean(RegEx(rx.STAR, ex))

		elif instr(self.stack, 1) == "+":
			self.stack.pop()
			self.clean(RegEx(rx.PLUS, ex))

		elif instr(self.stack, 1) == "?":
			self.stack.pop()
			self.clean(RegEx(rx.MAYBE, ex))

		elif instr(self.stack, 2) == "{":
			ran = self.stack.pop()
			self.stack.pop()
			self.clean(RegEx(rx.RANGE, ex, ran))

		elif instr(self.stack, 2) == "|" and not(isinstance(instr(self.stack, 1), str)):
			ex1 = self.stack.pop()
			self.stack.pop()
			self.clean(RegEx(rx.ALTERNATION, ex1, ex))

		elif instr(self.stack, 2) == "." and not(isinstance(instr(self.stack, 1), str)):
			ex1 = self.stack.pop()
			self.stack.pop()
			self.clean(RegEx(rx.CONCATENATION, ex1, ex))

		else:
			self.stack.append(ex)

	#ALTERNATION
	def enterR1(self, ctx):
		if ctx.r1() != None:
			self.stack.append("|")

	#CONCATENATION
	def enterR2(self, ctx):
		if ctx.r2() != None:
			self.stack.append(".")

	#STAR / PLUS / MAYBE / RANGE de orice tip (fixed, min , max, interval)
	def enterR3(self, ctx):
		if ctx.SYMBOL() != None:
			self.stack.append(RegEx(rx.SYMBOL_SIMPLE, toString(ctx.SYMBOL())))

		if ctx.star() != None:
			self.stack.append("*")

		if ctx.plus() != None:
			self.stack.append("+")

		if ctx.maybe() != None:
			self.stack.append("?")

		if ctx.range_0() != None:
			self.stack.append("{")
			if ctx.range_0().fixed() != None:
				num = getNumber(ctx.range_0().fixed().R_FIXED());
				self.stack.append((num, num))

			if ctx.range_0().min_0() != None:
				num = getFirstNumber(ctx.range_0().min_0().R_MIN());
				self.stack.append((num, -1))

			if ctx.range_0().max_0() != None:
				num = getLastNumber(ctx.range_0().max_0().R_MAX());
				self.stack.append((-1, num))

			if ctx.range_0().interval() != None:
				num1 = getFirstNumber(ctx.range_0().interval().R_INTERVAL());
				num2 = getLastNumber(ctx.range_0().interval().R_INTERVAL());
				self.stack.append((num1, num2))

	#s-a citit o sub-expresie, se evalueaza noua expresie
	def exitR3(self, ctx):
		ex = self.stack.pop()
		self.clean(ex)

	#se dechide o paranteza
	def enterGroup(self, ctx):
		self.stack.append("(")

	#se inchide o paranteza
	def exitGroup(self, ctx):
		val = self.stack.pop()
		self.stack.pop()
		self.stack.append(val);

	#ANY
	def enterAny_0(self, ctx):
		self.stack.append(RegEx(rx.SYMBOL_ANY))

	#SET
	def enterSet_0(self, ctx):
		self.stack.append("[")

	#se termina de citit setul
	def exitSet_0(self, ctx):
		self.stack.pop()
		self.stack.append(RegEx(rx.SYMBOL_SET, self.set.copy()))
		self.set.clear()

	def enterSet_2(self, ctx):
		if ctx.SYMBOL() != None:
			self.set.add(toString(ctx.SYMBOL()))

	def enterSet_3(self, ctx):
		self.set.add((toString(ctx.symbol_0().SYMBOL()), toString(ctx.symbol_1().SYMBOL())))

