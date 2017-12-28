#include "ASTree.h"

DataType Integer::getType()
{
	return DataType();
}

void Variable::clear()
{
}

bool Variable::isNil()
{
	return false;
}

bool Variable::visited()
{
	return false;
}

Variable IdC::evaluation()
{
	return Variable();
}

Variable LiteralC::evaluation()
{
	return Variable();
}

Variable VDefinationC::evaluation()
{
	return Variable();
}

Variable PreIncrementC::evaluation()
{
	return Variable();
}

Variable PreDecrementC::evaluation()
{
	return Variable();
}

Variable PositiveC::evaluation()
{
	return Variable();
}

Variable NegativeC::evaluation()
{
	return Variable();
}

Variable NegationC::evaluation()
{
	return Variable();
}

Variable PostIncrementC::evaluation()
{
	return Variable();
}

Variable PostDecrementC::evaluation()
{
	return Variable();
}

Variable PrimaryC::evaluation()
{
	return Variable();
}

Variable AddC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable SubC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable MultiC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable DiviC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable ModC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable AssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable MemberC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable CommaC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable GreaterC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable LessC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable EqualC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable NotEqualC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable GreaterEqualC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable LessEqualC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable AndC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable OrC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable LeftCopyC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable RightCopyC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable AddAssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable SubAssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable MultiAssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable DiviAssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable ModAssignC::evaluation(ASTree l, ASTree r)
{
	return Variable();
}

Variable ExprC::evaluation()
{
	return Variable();
}

Variable TriC::evaluation()
{
	return Variable();
}

Variable StatementC::evaluation()
{
	return Variable();
}

Variable BlockC::evaluation()
{
	return Variable();
}

Variable ReturnStateC::evaluation()
{
	return Variable();
}

Variable IfStateC::evaluation()
{
	return Variable();
}

Variable WhileStateC::evaluation()
{
	return Variable();
}

Variable DoWhileStateC::evaluation()
{
	return Variable();
}

Variable ForStateC::evaluation()
{
	return Variable();
}

Variable ForEachC::evaluation()
{
	return Variable();
}

Variable ArrayAccessC::evaluation()
{
	return Variable();
}

Variable FunCallC::evaluation()
{
	return Variable();
}

Variable MemFunCall::evaluation()
{
	return Variable();
}

Variable FunDefinationC::evaluation()
{
	return Variable();
}

Variable ArrayDefination::evaluation()
{
	return Variable();
}
