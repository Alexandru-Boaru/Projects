import regex as rx
import regular_expression as re

from regex import RegEx
from regular_expression import RegularExpression
from nfa import NFA
from dfa import DFA

#conversie set in reuniuni recursive de simboluri
def get_set(tuple, temp):
	(begin, end) = tuple;
	for i in range(rx.CHARSET.index(begin), rx.CHARSET.index(end)+1):
		if(temp == None):
			temp = RegularExpression(re.SYMBOL, rx.CHARSET[i]);
		else:
			e0 = temp;
			e1 = RegularExpression(re.SYMBOL, rx.CHARSET[i]);
			temp = RegularExpression(re.ALTERNATION, e0, e1);

	return temp;

any_symbol_reg_expr = get_set((rx.CHARSET[0], rx.CHARSET[-1]), None)

#redenumire stari
def rename_states(target, reference):
    off = max(reference.states) + 1
    target.start_state += off
    target.states = set(map(lambda s: s + off, target.states))
    target.final_states = set(map(lambda s: s + off, target.final_states))
    new_delta = {}
    for (state, symbol), next_states in target.delta.items():
        new_next_states = set(map(lambda s: s + off, next_states))
        new_delta[(state + off, symbol)] = new_next_states

    target.delta = new_delta

#stari noi
def new_states(*nfas):
    state = 0
    for nfa in nfas:
        m = max(nfa.states)
        if m >= state:
            state = m + 1

    return state, state + 1


# conversie RegEx -> RegularExpression
def regex_to_regular_expr(regex):
	if regex is None:
		return RegularExpression(re.EMPTY_SET);

	elif(regex.type == rx.EMPTY_STRING):
		return RegularExpression(re.EMPTY_STRING);

	elif(regex.type == rx.SYMBOL_SIMPLE):
		return RegularExpression(re.SYMBOL, regex.symbol);

	elif(regex.type == rx.SYMBOL_ANY):
		return any_symbol_reg_expr;

	elif(regex.type == rx.SYMBOL_SET):
		chars = regex.symbol_set;
		temp = None;
		for c in chars:
			if(isinstance(c,str)):
				if(temp == None):
					temp = RegularExpression(re.SYMBOL, c);
				else:
					e0 = temp;
					e1 = RegularExpression(re.SYMBOL, c);
					temp = RegularExpression(re.ALTERNATION, e0, e1);
			elif(isinstance(c,tuple)):
				temp = get_set(c, temp);

		return temp;

	elif(regex.type == rx.MAYBE):
		e = regex_to_regular_expr(regex.lhs);
		empty = RegularExpression(re.EMPTY_STRING);
		return RegularExpression(re.ALTERNATION, e, empty);

	elif(regex.type == rx.STAR):
		e = regex_to_regular_expr(regex.lhs);
		return RegularExpression(re.STAR, e);

	elif(regex.type == rx.PLUS):
		e = regex_to_regular_expr(regex.lhs);
		star_e = RegularExpression(re.STAR, e);
		return RegularExpression(re.CONCATENATION, e, star_e);

	elif(regex.type == rx.RANGE):
		e = regex_to_regular_expr(regex.lhs);
		final = RegularExpression(re.EMPTY_SET);
		(begin, end) = regex.range;

		if(begin == -1):
			begin = 0;
		
		temp = RegularExpression(re.EMPTY_STRING);

		for i in range(0,begin):
			e1 = temp;
			if(temp.type == re.EMPTY_STRING):
				temp = e;
			else:
				temp = RegularExpression(re.CONCATENATION, e1, e);

		if(end == -1):
			e1 = temp;
			star_e = RegularExpression(re.STAR, e);
			if(temp.type == re.EMPTY_STRING):
				temp = star_e;
			else:
				temp = RegularExpression(re.CONCATENATION, e1, star_e);
			final = temp;

		else:
			final = temp;
			rep1 = temp;
			for i in range(begin, end):
				temp = final;
				rep0 = rep1;
				if(rep0.type == re.EMPTY_STRING):
					rep1 = e;
				else:
					rep1 = RegularExpression(re.CONCATENATION, rep0, e);
				final = RegularExpression(re.ALTERNATION, temp, rep1);

		return final

	elif(regex.type == rx.CONCATENATION):
		e0 = regex_to_regular_expr(regex.lhs);
		e1 = regex_to_regular_expr(regex.rhs);
		return RegularExpression(re.CONCATENATION, e0, e1);

	elif(regex.type == rx.ALTERNATION):
		e0 = regex_to_regular_expr(regex.lhs);
		e1 = regex_to_regular_expr(regex.rhs);
		return RegularExpression(re.ALTERNATION, e0, e1);

# conversie RegularExpression -> NFA
def re_to_nfa(reg_exp):

    if(reg_exp.type == re.EMPTY_SET):
      	return NFA(rx.CHARSET, {0, 1}, 0, {1}, {})

    elif(reg_exp.type == re.EMPTY_STRING):
      	return NFA(rx.CHARSET, {0, 1}, 0, {1}, {(0, ""): {1}})

    elif(reg_exp.type == re.SYMBOL):
      	return NFA(rx.CHARSET, {0, 1}, 0, {1}, {(0, reg_exp.symbol): {1}})

    elif(reg_exp.type == re.STAR):
      	temp = re_to_nfa(reg_exp.lhs)
      	state1, state2 = new_states(temp)
      
      	(f, ) = temp.final_states
      	new_delta = {**temp.delta}
      	new_delta.update({(state1, ""): {state2, temp.start_state}})
      	new_delta.update({(f, ""): {state2, temp.start_state}})
      
      	nfa_states = set(temp.states)
      	nfa_states.add(state1)
      	nfa_states.add(state2)
      
      	new_start_state = state1
      
      	new_final_states = {state2}
      
      	return NFA(rx.CHARSET, nfa_states, new_start_state, new_final_states, new_delta)
    
    elif(reg_exp.type == re.CONCATENATION):
	    nfa1 = re_to_nfa(reg_exp.lhs)
	    nfa2 = re_to_nfa(reg_exp.rhs)
	    
	    rename_states(nfa2, nfa1)
	    state1, state2 = new_states(nfa1, nfa2)
	     
	    (f1, ) = nfa1.final_states
	    (f2, ) = nfa2.final_states
	      
	    new_delta = {**nfa1.delta, **nfa2.delta}
	    new_delta.update({(state1, ""): {nfa1.start_state}})
	    new_delta.update({(f1, ""): {nfa2.start_state}})
	    new_delta.update({(f2, ""): {state2}})
	     
	    new_alphabet = nfa1.alphabet + nfa2.alphabet
	     
	    nfa_states = nfa1.states.union(nfa2.states)
	    nfa_states.add(state1)
	    nfa_states.add(state2)
	     
	    new_start_state = state1
	     
	    new_final_states = {state2}
	      
	    return NFA(rx.CHARSET, nfa_states, new_start_state, new_final_states, new_delta)
    
    elif(reg_exp.type == re.ALTERNATION):
      	nfa1 = re_to_nfa(reg_exp.lhs)
      	nfa2 = re_to_nfa(reg_exp.rhs)
      
      	rename_states(nfa2, nfa1)
      	state1, state2 = new_states(nfa1, nfa2)
      	
      	(f1, ) = nfa1.final_states
      	(f2, ) = nfa2.final_states
      	new_delta = {**nfa1.delta, **nfa2.delta}
      	new_delta.update({(state1, ""): {nfa1.start_state, nfa2.start_state}})
      	new_delta.update({(f1, ""): {state2}})
      	new_delta.update({(f2, ""): {state2}})
      
      	new_alphabet = nfa1.alphabet + nfa2.alphabet
      
      	nfa_states = nfa1.states.union(nfa2.states)
      	nfa_states.add(state1)
      	nfa_states.add(state2)
      
      	new_start_state = state1
      
      	new_final_states = {state2}
      
      	return NFA(rx.CHARSET, nfa_states, new_start_state, new_final_states, new_delta)


def epsilon_closure(states, delta):
	if(states == None):
		return None;
	eps = states;
	checked = set();
	while(len(eps)>0):
		temp = eps.pop()
		if temp in checked:
			continue;
		eps_states = delta.get((temp,""));
		if(eps_states==None):
			checked.add(temp);
			continue
		eps = eps.union(eps_states);
		checked.add(temp);
	if(checked==set()):
		checked=None;
	return checked;

# conversie NFA -> DFA
def nfa_to_dfa(nfa):
	new_delta = dict();
	new_states = set();
	newfound_sets = dict();
	checked_sets = dict();
	new_final_states = set();
	counter = 0;
	start_point = epsilon_closure({nfa.start_state}, nfa.delta);
	newfound_sets[counter] = start_point;
	new_states.add(counter);
	counter = counter+1;

	while(len(newfound_sets)>0):
		(num, current_state) = newfound_sets.popitem();
		next_state = set();

		for c in nfa.alphabet:
			next_state = set();
			for s in current_state:
				temp = None
				if(nfa.delta.get((s,c)) != None):
					temp = nfa.delta.get((s,c)).copy();

				if(temp!=None and next_state!=None):
					if(len(temp) > 0):
						next_state.add(temp.pop())
			next_state = epsilon_closure(next_state,nfa.delta);
			if(next_state == None or len(next_state) <= 0):
				new_delta[(num,c)] = -1;
			elif(current_state == next_state):
				new_delta[(num,c)] = num;
			elif(next_state in newfound_sets.values()):
				new_delta[(num,c)] = list(newfound_sets.keys())[list(newfound_sets.values()).index(next_state)];
			elif(next_state in checked_sets.values()):
				new_delta[(num,c)] = list(checked_sets.keys())[list(checked_sets.values()).index(next_state)];
			else:
				newfound_sets[counter] = next_state;
				new_delta[(num,c)] = counter;
				new_states.add(counter);
				counter = counter+1;

		checked_sets[num] = current_state;

	for c in rx.CHARSET:
		new_delta[(-1,c)] = -1;

	for (i, subS) in checked_sets.items():
		if(len(subS.intersection(nfa.final_states))>0):
			new_final_states.add(i);


	return DFA(nfa.alphabet, new_states, 0, new_final_states, new_delta);

#conversie RegEx -> DFA
def regex_to_dfa(rex):
	reg = regex_to_regular_expr(rex);
	n = re_to_nfa(reg);
	d = nfa_to_dfa(n);
	return (n,d);

#verifica daca cuvantul dat face match pe expresie
def check_word(word, dfa):
	current_state = dfa.start_state;
	for c in word:
		if(c=="\n"):
			break;
		current_state = dfa.delta[(current_state,c)]
	if(dfa.final_states.issuperset({current_state})):
		print(True);
	else:
		print(False);
