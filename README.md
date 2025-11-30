# compiler

## 1 介绍

天津大学编译原理与技术结课编译器前端设计

## 2 软件架构

### 2.1 目录结构

```
compiler/
├── build/                  # 编译时产生的存放编译结果的目录
├── case/                  # 测试用例目录
│   ├── test.sy           # 测试用例C--源码
│   ├── test_token_result.txt     # 测试用例词法解析结果
│   ├── test_parse_analysis.txt   # 测试用例语法分析过程
│   ├── test.parse_tree.md        # 测试用例语法分析树
│   └── test_semantic_tree.txt    # 语义分析结果
├── documents/             # 文档目录
|   ├── note.md            # 一些记录
│   └── 2025秋季编译原理大作业.pdf
├── include/               # 头文件目录
│   ├── ast.h        # 目前没有使用，暂存
│   ├── lexer.h           # 词法分析器头文件
│   ├── parser.h           # 语法分析器头文件
│   ├── semantic_analyzer.h     # 语义分析器头文件
│   ├── symbol_table.h    # 符号表头文件
│   └── ir_generator.h     # 中间代码调用头文件
├── ir_lib/               # 中间代码生成外部库
├── logs/                 # 记录每次运行信息的日志目录
├── scripts/               # 脚本和工具目录
│   ├── parser_usage_example.cpp # 用于测试语法分析树获取是否可以被正常调用
│   └── ...         # 其他调试用的测试文件
├── src/                   # 源代码目录
│   ├── ast.cpp     # 目前没有使用，暂存
│   ├── ir_generator.cpp     # 中间代码调用实现
│   ├── lexer.cpp         # 词法分析器实现
│   ├── parser.cpp           # 语法分析器实现
│   ├── semantic_analyzer.cpp     # 语义分析器实现
│   ├── symbol_table.cpp    # 符号表，供ir_generator使用，具体实现在.h文件
│   └── rule.txt          # 文法原始产生式
├── process/              # 分析器构建的输出(文法和词法规则不变就是定死的)
│   ├── first_sets.txt    # 语法分析FIRST集合
│   ├── follow_sets.txt    # 语法分析FOLLOW集合
│   ├── parse_analysis_table.csv    # 语法分析表
│   └── lexer_state_transition_matrix.csv      # 词法分析DFA状态转移矩阵
├── Makefile
├── .gitignore
└── README.md
```

## 3 运行启动

所有编译产生的结果位于 `/build` 下

以下仅列出一些常用命令，具体直接看 `Makefile` 就行

```bash
make clean

# 编译到词法分析器 (lexer.exe)
make lexer
./build/lexer.exe \<可选测试用例文件\>

# 编译到语法分析器 (parser.exe)
make parser
./build/parser.exe \<可选测试用例文件\>

# 编译整个项目(包括中间代码部分的)
make compiler
./build/compiler.exe \<可选测试用例文件\>

# 例如：
./build/compiler.exe .\case\test1.sy
```

每次分析会在用例路径也就是 `/case` 输出：

- 词法分析 token 解析结果：`<用例文件名>_token_result.txt`
- 语法分析过程：`<用例文件名>_parse_anlysis.txt`
- 语法分析树：`<用例文件名>_parse_tree.md`
- 语义结果：`<用例文件名>_semantic_tree.txt`
- LLVM输出：`<用例文件名>_output.ll`

每次运行都会将过程调试信息输出至 `/logs` 目录下

- 这个目录在 make 时如果不存在就会自动创建
- 日志文件名按时间戳命名
- 部分过程表示输出以及语义报错输出到终端

## 4 开发说明

- 目前带有 `#EXPORT_DEBUG#` 的注释下是调用一些输出构建信息的函数，这些构建信息已经预先运行一遍存在 `/process` 目录下了，如果后续有修改构建需要重新输出，直接查找这个标记取消相应注释就ok了

- 关于 "main" 这个要求文档上词法分析把这个弄成KW，但是后续分析类型解析成标识符比较好，所以token输出的是KW(给老师看的)，实际上都是按照标识符使用的

## 5 一些其他说明



### 5.1 联系方式

zengyicydd@tju.edu.cn

gaocan@tju.edu.cn

daimingjing142857@tju.edu.cn

jsyy@tju.edu.cn