#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include <iostream>
#include <string>
#include <vector>

#include "environment.hpp"

struct ExpressionNode {
  virtual ~ExpressionNode() {};
  virtual int evaluate(Environment &env) = 0;
};

struct PrintNode : public ExpressionNode {
  PrintNode(ExpressionNode *value) : value{value} {}

  int evaluate(Environment &env) override {
    printf("%d\n", value->evaluate(env));
    return 0;
  }

  ExpressionNode *value = nullptr;
};

struct NumberNode : public ExpressionNode {
  NumberNode(int v) : value{v} {}

  int evaluate(Environment &env) override { return value; }

  int value;
};

struct PlusNode : public ExpressionNode {
  PlusNode(ExpressionNode *lhs, ExpressionNode *rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) + right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct MinusNode : public ExpressionNode {
  MinusNode(ExpressionNode *lhs, ExpressionNode *rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) - right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct MultNode : public ExpressionNode {
  MultNode(ExpressionNode *lhs, ExpressionNode *rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) * right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct DivNode : public ExpressionNode {
  DivNode(ExpressionNode *lhs, ExpressionNode *rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) / right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct LessThanNode : public ExpressionNode {
  LessThanNode(ExpressionNode *lhs, ExpressionNode *rhs)
      : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) < right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct LeqThanNode : public ExpressionNode {
  LeqThanNode(ExpressionNode *lhs, ExpressionNode *rhs)
      : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) <= right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct GeqThanNode : public ExpressionNode {
  GeqThanNode(ExpressionNode *lhs, ExpressionNode *rhs)
      : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) >= right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct GreaterThanNode : public ExpressionNode {
  GreaterThanNode(ExpressionNode *lhs, ExpressionNode *rhs)
      : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) > right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct NotEqualNode : public ExpressionNode {
  NotEqualNode(ExpressionNode *lhs, ExpressionNode *rhs)
      : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) != right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};

struct EqualNode : public ExpressionNode {
  EqualNode(ExpressionNode *lhs, ExpressionNode *rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment &env) override {
    return left->evaluate(env) == right->evaluate(env);
  }

private:
  ExpressionNode *left;
  ExpressionNode *right;
};


struct ReturnNode : public ExpressionNode {
  ReturnNode(ExpressionNode* expr_): expr{expr_} {}
  int evaluate(Environment& env) override {
    return expr->evaluate(env);
  }

  private:
    ExpressionNode* expr;
};



struct AssignmentNode : public ExpressionNode {
  AssignmentNode(char *lhs, ExpressionNode *rhs) : id{lhs}, value{rhs} {}
  int evaluate(Environment &env) override {
    if (typeid(value) == typeid(ReturnNode)) {
      return env.vars[id] = value->evaluate(env);
    } else {
    return env.vars[id] = value->evaluate(env);
    }
  }

private:
  char *id;
  ExpressionNode *value;
};

struct VariableNode : public ExpressionNode {
  VariableNode(char *id_) : id{id_} {}
  int evaluate(Environment &env) override { return env.vars[id]; }

private:
  char *id;
};

struct IfThenElseNode : public ExpressionNode {
  IfThenElseNode(ExpressionNode *condition_, ExpressionNode *body_true_,
                 ExpressionNode *body_false_)
      : condition{condition_}, body_true{body_true_}, body_false{body_false_} {}
  int evaluate(Environment &env) override {
    if (condition->evaluate(env)) {
      return body_true->evaluate(env);
    } else {
      if (body_false != nullptr) {
        return body_false->evaluate(env);
      }
      return 0;
    }
  }

private:
  ExpressionNode *condition;
  ExpressionNode *body_true;
  ExpressionNode *body_false;
};

struct WhileDoNode : ExpressionNode {
  WhileDoNode(ExpressionNode *condition_, ExpressionNode *body_)
      : condition{condition_}, body{body_} {}
  int evaluate(Environment &env) override {
    while (condition->evaluate(env)) {
      body->evaluate(env);
    }
    return 0;
  }

private:
  ExpressionNode *condition;
  ExpressionNode *body;
};

struct BlockCodeNode : public ExpressionNode {
  BlockCodeNode(std::vector<ExpressionNode *> *exprs_) : exprs{exprs_} {}
  int evaluate(Environment &env) override {
    for (const auto &x : *exprs) {
      x->evaluate(env);
    }
    return 0;
  }

private:
  std::vector<ExpressionNode *> *exprs;
};

struct FunctionNode : public ExpressionNode {
  FunctionNode(std::vector<std::string> *args_,
               std::vector<ExpressionNode *> *body_)
      : args{args_}, body{body_} {}
  FunctionNode() : args{nullptr}, body{nullptr} {}

  int evaluate(Environment &env) override {
    for (const auto &expr : *body) {
      expr->evaluate(env);
    }
    return 0;
  }

  std::vector<std::string> *args;
  std::vector<ExpressionNode *> *body;
};

struct FunctionDefinitionNode : public ExpressionNode {
  FunctionDefinitionNode(std::string fn_name_, std::vector<std::string> *args_,
                         std::vector<ExpressionNode *> *body_)
      : fn_name{fn_name_}, args{args_}, body{body_} {}
  int evaluate(Environment &env) override {
    FunctionNode fnode = FunctionNode{args, body};
    env.funcs[fn_name] = fnode;
    return 0;
  }

  std::string fn_name;
  std::vector<std::string> *args;
  std::vector<ExpressionNode *> *body;
};




struct FunctionCallNode : public ExpressionNode {
  FunctionCallNode(std::string fn_name_, std::vector<ExpressionNode *> *body_)
      : fn_name{fn_name_}, body{body_} {}
  int evaluate(Environment &env) override {
    FunctionNode currentFunction = env.funcs[fn_name];
    if (env.funcs.find(fn_name) != env.funcs.end()) {
    for (int i = 0; i < currentFunction.args->size(); ++i) {
      env.vars[currentFunction.args->at(i)] = body->at(i)->evaluate(env);
    }
    for (int i = 0; i < currentFunction.body->size(); ++i) {
      currentFunction.body->at(i)->evaluate(env);
    }
    }

    ExpressionNode* lastExpr = currentFunction.body->at(currentFunction.body->size()-1);
    if (typeid(*lastExpr) != typeid(PrintNode) ) {
      return currentFunction.body->at(currentFunction.body->size()-1)->evaluate(env);
      }
    else return 0;

  }

private:
  std::string fn_name;
  std::vector<ExpressionNode *> *body;
};
// implementirati

#endif // EXPRESSION_TREE_H
