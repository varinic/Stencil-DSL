# StencilDSL

## IRNode

1. 抽象语法树的基类

2. 通过accept()函数接收IRVisitor

   1. ```
      class IRNode {
       public: 
        Block * parent;
        IRNode * get_ir_root();
        virtual void accept(IRVisitor *visitor) {}
        virtual ~IRNode() = default;
      };
      ```

## IRVisitor

1. 遍历抽象语法树的基类

2. virtual void visit(***)通过虚函数实现调用不同方法（多态）

   1. ```
      class IRVisitor {
       public:
        IRVisitor() {}
        virtual void visit(Stmt *stmt) {}
        virtual void visit(FuncOpExpr *op) {}
        virtual void visit(AssignOpExpr *op) {}
        virtual void visit(BinaryOpExpr *op) {}
        virtual void visit(UnaryOpExpr *op) {}
        ...
      };
      ```

   2. ```
      #define DEFINE_ACCEPT                        \
        void accept(IRVisitor *visitor) override { \
          visitor->visit(this);                    \
        }
      ```

   3. 每个IRNode的派生类有accept方法来接收IRVisitor，通过visit虚函数来实现调用不同方法，this指针为指向当前class的指针

## IRBuilder

1. 存放抽象语法树

2. ```
   class IRBuilder {
    private:
     std::vector<std::shared_ptr<Block> > stack;
     ...
   }
   ```

## 派生关系

1. IRNode
   1. Stmt
      1. FrontendAllocStmt
      2. FrontendForStmt
      3. ...
   2. Block
   3. Expr
      1. Variable
      2. FuncOp
      3. AssignedOp
      4. BinaryOp
      5. UnaryOp
2. IRVisitor
   1. CodeGenCPU
   2. CodeGenSunway
   3. CodeGenSunwaySlave

## 派生类

1. ExprGroup
   1. 逗号表达式
2. SpNode
3. SpNodeIndicesExpr
   1. SpNode下标
4. FrontendTensor
   1. 唯一表示用户定义的张量
5. TeNode
   1. 中间结果
6. TeNodeIndicesExpr
   1. TeNode下标
7. Input
8. Output
9. MPIExpr
10. CallFuncExpr
    1. 调用函数
11. CallNodeExpr
    1. 调用TeNode
12. FunctionStmt
    1. 函数
13. RangeForStmt
    1. 单个for循环
14. FrontendForStmt
    1. 多层嵌套循环，不可split
15. ScheduleForStmt
    1. 多层嵌套循环，可split
16. ...

## ScheduleForStmt

1. Axis
   1. 轴，表示ScheduleForStmt中的可split的循环
      1. id_var
      2. order
      3. split_flag
      4. begin
      5. end
      6. stride
      7. ...
2. CacheRead/CacheWrite
   1. 读写缓存
      1. SpNode/TeNode
      2. Axis
      3. allocation
3. compute_at
   1. 设置读写缓存的axis




## SpNode

### 属性

1. ndim：维度个数
2. shape：各个维度的大小
3. halo_size：根据stencil在各个维度上的数据访问来统计各个维度的halo区大小
4. dt：数据类型

### 运算规则

1. SpNode之间可以进行常规的算术运算（arithmetic），但是两个SpNode的shape要完全匹配。（注：这里使用的是强数据类型，不像numpy中的多维数组，部分array的shape不匹配会自动补零）
2. scalar variable与SpNode之间可以进行算术运算，默认scalar与SpNode逐点进行运算。
3. 系数向量，

### Index设计

1. 通过下标访问离散数据。
2. IndexExpression表示下标运算，继承自Expression，主要支持加减法。

### IR

IRNode
    Expr 表示表达式的节点
        VariableExpr 变量表达式
        FuncOpExpr 数学函数表达式
        AssignOpExpr 赋值表达式
        BinaryOpExpr 二元表达式
        UnaryOpExpr 一元表达式
        CallFuncExpr 函数调用表达式
        SpNode  用以表示空间维度
        SpNodeIndicesExpr 空间维度及其下标
        TeNodeExpr 时间维度
    Stmt 表示语句的节点
        FrontendForStmt 多层循环表示空间维度的遍历
        FunctionStmt 函数语句
        FrontendAllocaStmt 变量和张量的定义与分配空间
    Block 表示程序块

### 注意事项
1. macos上默认的g++调用的clang，不带openmp库，需要自己手动安装openmp库，brew install libomp。系统层面配置完openmp环境，才能将生成的代码编译成多线程程序。

