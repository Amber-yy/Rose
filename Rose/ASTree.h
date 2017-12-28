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

/*�����﷨���������﷨�ṹ�Ļ���*/
class ASTreeC
{
public:
	virtual Variable evaluation() = 0;
	Rose *rose;
};

using ASTree=std::shared_ptr < ASTreeC >;

/*
��ʾ��ʶ����Ԫ�أ�name�Ǳ�ʶ������
level��ʾ�˱�ʶ����Ӧ�ı�������level�����������
index�������䱾����������е��±�
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
��ʾ�����������������������ַ�����������
ֱ�Ӹ��ݺ��������ú���ʱ�������������������Ǳ�ʶ����
ͨ�����������ú���ʱ����Id��Id��ӦVariable��Variable�б����ź���������
*/
class LiteralC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	Variable value;
};

using Literal = std::shared_ptr < LiteralC >;

/*������������һ�пɰ������������������������ʱ������Ӧ�ı���*/
class VDefinationC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::vector<std::string> names;
};

using VDefination = std::shared_ptr < VDefinationC >;

/*
��Ŀ�����Ԫ�أ�����������ɣ�
��Ŀ�����+Primary
��Ŀ���������Ϊ���ֻ��࣬Primary����Ϊ��Ա
*/
class UnaryC :public ASTreeC
{

};

/*ǰ�����������*/
class PreIncrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*ǰ���Լ������*/
class PreDecrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*����*/
class PositiveC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*����*/
class NegativeC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*�߼���*/
class NegationC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*�������������*/
class PostIncrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

/*�����Լ������*/
class PostDecrementC :public UnaryC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Unary = std::shared_ptr < UnaryC >;

/*
����Ԫ�أ��ɱ�ʶ��������������Բ���ŵı��ʽ����Ŀ�������
�������á��������֮һ����
*/
class PrimaryC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Primary = std::shared_ptr < PrimaryC >;

/*����˫Ŀ������Ļ���*/
class BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) = 0;
};

/*�ӷ�����*/
class AddC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*��������*/
class SubC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*�˷�����*/
class MultiC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*��������*/
class DiviC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*ȡģ����*/
class ModC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*��ֵ����*/
class AssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*��Ա�����*/
class MemberC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*���������*/
class CommaC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*����*/
class GreaterC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*С��*/
class LessC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*���*/
class EqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*������*/
class NotEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*���ڵ���*/
class GreaterEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*С�ڵ���*/
class LessEqualC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*�߼���*/
class AndC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*�߼���*/
class OrC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*ֵ��������*/
class LeftCopyC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*ֵ�������ң�*/
class RightCopyC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*�ӷ���ֵ*/
class AddAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*������ֵ*/
class SubAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*�˷���ֵ*/
class MultiAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*������ֵ*/
class DiviAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

/*ȡģ��ֵ*/
class ModAssignC :public BinaryC
{
public:
	virtual Variable evaluation(ASTree l, ASTree r) override;
};

using Binary = std::shared_ptr < BinaryC >;

/*
���ʽ��������ʽ ˫Ŀ����� �ұ��ʽ���
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

/*�ڷ������ʽ��ʱ�����������Ŀ��������򽫱��ʽ����Ϊ��Ŀ�����*/
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

/*���*/
class StatementC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	ASTree value;
};

using Statement = std::shared_ptr < StatementC >;

/*���飬Ҳ��һ�����������п��Ժ��ж�����������*/
class BlockC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
	std::vector<ASTree> statements;
};

using Block = std::shared_ptr < BlockC >;

/*
return ��䣬����ǰ��Runner��isCalling�ֶ�����Ϊfalse����ֹ��ǰ�����ĵ��ã�
���������ֵ�ŵ�Runner�У���Ϊ��ǰ�����ķ���ֵ
*/
class ReturnStateC :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	Expr value;
};

/*
if��䣬
���ж�������Ȼ����ִ��states��estates����ֵ���Ϊһ���յı���
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
while��䣬
���ж�������Ȼ�������������٣��ж��Ƿ�ִ��states��
������break����isLooping��Ϊfalse
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
������while������Ҫ��ִ��һ��states
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
����ִ��ini��
Ȼ���ж�condition�������Ƿ�ִ��state,
һ��ѭ��֮��ִ��change
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

/*����container*/
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

/*���ȼ���array���ʽ���õ����飬Ȼ�����index����󷵻�array.index*/
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
���ȼ���function�ĵ���Ӧ�ĺ�����Ȼ�����parameters��
�������һ���ֲ�������ӳ�����飬�������鴫�ݸ�Runner
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
��Ա�������ã����ȸ���Id�����������
Ȼ���������Ϻ��������õ�ӳ����еĺ�����
������������б�Ȼ����ú���

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
�����ĺ���������һ�����飬ִ��һ�θ�����
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
���鶨��
*/
class ArrayDefination :public ASTreeC
{
public:
	virtual Variable evaluation()override;
protected:
	std::vector<Expr> exprs;
};

