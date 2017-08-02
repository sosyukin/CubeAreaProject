#pragma once
class _CAEnglishGrammarExplanation
{
	class EGENounModule {};
	class EGEVerbModule 
	{
		class VerbCore {};
		VerbCore be;
	};
	class EGEAdverbialModule {};
	typedef EGENounModule N;
	typedef EGEVerbModule V;
	typedef EGEAdverbialModule Adverbial;
	class EGESentence
	{
		V v;
		N n;
		Adverbial adverbial;
	};
public:
	_CAEnglishGrammarExplanation();
	~_CAEnglishGrammarExplanation();
};

