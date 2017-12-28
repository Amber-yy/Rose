#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>

enum DataType
{
	INT,
	REAL,
	STRING,
	ARRAY,
	MAP,
	OBJECT,
	FUNCTION,
	USERDATA
};

class Rose;

class Data
{
public:
	virtual DataType getType() = 0;
	virtual ~Data(){}
	bool visited;
	int counter;
};

class Integer :public Data
{
public:
	virtual DataType getType()override;
	long long value;
};

class Variable
{
public:
	void clear();
	bool isNil();
	bool visited();
	Data *data;
};

using Iterator = std::list<Variable>::iterator;

/*抽象语法树，所有语法结构的基类*/
class ASTreeC
{
public:
	virtual Variable evaluation() = 0;
	Rose *rose;
};

using ASTree=std::shared_ptr < ASTreeC >;

/*
表示标识符的元素，name是标识符名，
level表示此标识符对应的变量属于level层外的作用域，
index变量在其本身的作用域中的下标
*/

class IdC:public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::string name;
	int level;
	int index;
};

using Id = std::shared_ptr < IdC >;

/*
表示字面量，整数、浮点数、字符串、函数等
直接根据函数名调用函数时，函数名是字面量而非标识符，
通过变量来调用函数时才是Id，Id对应Variable，Variable中保存着函数的索引
*/
class LiteralC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	Variable value;
};

using Literal = std::shared_ptr < LiteralC >;

/*变量的声明，一行可包含多个声明，遇到变量声明时创建对应的变量*/
class VDefinationC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::vector<std::string> names;
};

using VDefination = std::shared_ptr < VDefinationC >;

/*
单目运算符元素，由两部分组成：
单目运算符+Primary
单目运算符体现为各种基类，Primary则作为成员
*/
class UnaryC :public ASTreeC
{

};

/*前置自增运算符*/
class PreIncrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*前置自减运算符*/
class PreDecrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*正号*/
class PositiveC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*负号*/
class NegativeC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*逻辑非*/
class NegationC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*后置自增运算符*/
class PostIncrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*后置自减运算符*/
class PostDecrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Unary = std::shared_ptr < UnaryC >;

/*
基本元素，由标识符、字面量、带圆括号的表达式、单目运算符、
函数调用、数组访问之一构成
*/
class PrimaryC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Primary = std::shared_ptr < PrimaryC >;

/*所有双目运算符的基类*/
class BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) = 0;
};

/*加法运算*/
class AddC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*减法运算*/
class SubC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*乘法运算*/
class MultiC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*除法运算*/
class DiviC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*取模运算*/
class ModC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*赋值运算*/
class AssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*成员运算符*/
class MemberC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*逗号运算符*/
class CommaC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*大于*/
class GreaterC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*小于*/
class LessC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*相等*/
class EqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*不等于*/
class NotEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*大于等于*/
class GreaterEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*小于等于*/
class LessEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*逻辑与*/
class AndC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*逻辑或*/
class OrC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*值拷贝（左）*/
class LeftCopyC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*值拷贝（右）*/
class RightCopyC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*加法赋值*/
class AddAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*减法赋值*/
class SubAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*乘法赋值*/
class MultiAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*除法赋值*/
class DiviAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*取模赋值*/
class ModAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

using Binary = std::shared_ptr < BinaryC >;

/*
表达式：由左表达式 双目运算符 右表达式组成
*/
class ExprC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree left;
	Binary op;
	ASTree right;
};

using Expr = std::shared_ptr < ExprC >;

/*在分析表达式的时候，如果遇到三目运算符，则将表达式调整为三目运算符*/
class TriC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr condition;
	Expr first;
	Expr second;
};

using Tri = std::shared_ptr < TriC >;

/*语句*/
class StatementC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Statement = std::shared_ptr < StatementC >;

/*语句块，也是一个作用域，其中可以含有多个其它的语句*/
class BlockC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::vector<ASTree> statements;
};

using Block = std::shared_ptr < BlockC >;

/*
return 语句，将当前的Runner的isCalling字段设置为false，终止当前函数的调用，
并将求出的值放到Runner中，作为当前函数的返回值
*/
class ReturnStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr value;
};

/*
if语句，
先判断条件，然后再执行states或estates，求值结果为一个空的变量
*/
class IfStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr condition;
	ASTree states;
	ASTree eStates;
};

using IfState = std::shared_ptr < IfStateC >;

/*
while语句，
先判断条件，然后根据条件的真假，判断是否执行states，
若遇到break，则将isLooping设为false
*/
class WhileStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr condition;
	ASTree states;
};

using WhileState = std::shared_ptr < WhileStateC >;

/*
类似于while，但是要先执行一次states
*/
class DoWhileStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr condition;
	ASTree states;
};

using DoWhileState = std::shared_ptr < DoWhileStateC >;

/*
首先执行ini，
然后判断condition，决定是否执行state,
一次循环之后执行change
*/
class ForStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr ini;
	Expr condition;
	Expr change;
	ASTree states;
};

using ForState = std::shared_ptr < ForStateC >;

/*遍历container*/
class ForEachC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Id it;
	Id container;
	ASTree states;
};

using ForEach = std::shared_ptr < ForEachC >;

/*首先计算array表达式，得到数组，然后计算index，最后返回array.index*/
class ArrayAccessC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree array;
	Expr index;
};

using ArrayAccess = std::shared_ptr < ArrayAccessC >;

/*
首先计算function的到相应的函数，然后计算parameters，
最后设置一个局部变量的映射数组，将该数组传递给Runner
*/
class FunCallC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Primary function;
	ASTree parameters;
};

/*
成员函数调用，首先根据Id计算出类名，
然后将类名加上函数名，得到映射表中的函数名
最后计算出参数列表，然后调用函数

*/
class MemFunCall :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	std::string name;
	Id obj;
	ASTree parameters;
};

using FunCall = std::shared_ptr < FunCallC >;

/*
真正的函数，含有一个语句块，执行一次该语句块
*/
class FunDefinationC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::string name;
	Block block;
};

using FunDefination = std::shared_ptr < FunDefinationC >;

/*
数组定义
*/
class ArrayDefination :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	std::vector<Expr> exprs;
};

