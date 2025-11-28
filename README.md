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
│   ├── test_parse_analysis.txt      # 测试用例语法分析过程
│   └── test.parse_tree.md      # 测试用例语法分析树
├── documents/             # 文档目录
|   ├── note.md            # 一些记录
│   └── 2025秋季编译原理大作业.pdf
├── include/               # 头文件目录
│   ├── lexer.h           # 词法分析器头文件
│   ├── parser_class.h      # 语法分析器接口声明头文件
│   └── parser.h           # 语法分析器头文件
├── logs/                 # 记录每次运行信息的日志目录
├── scripts/               # 脚本和工具目录
│   └── test.cpp          # 测试脚本
├── src/                   # 源代码目录
│   ├── lexer.cpp         # 词法分析器实现
│   ├── parser.cpp           # 语法分析器实现
│   └── rule.txt          # 文法原始产生式
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

# 编译整个项目 (因为当前只写到语法分析，所以后续要完善Makefile的这个命令)
make all
```

每次分析会在用例路径也就是 `/case` 输出：
- 词法分析 token 解析结果：`<用例文件名>_token_result.txt`
- 语法分析过程：`<用例文件名>_parse_anlysis.txt`
- 语法分析树：`<用例文件名>_parse_tree.md`

每次运行都会将过程调试信息输出至 `/logs` 目录下
- 这个目录在 make 时如果不存在就会自动创建
- 日志文件名按时间戳命名

## 4 开发说明

目前带有 `#EXPORT_DEBUG#` 的注释下是调用一些输出构建信息的函数，这些构建信息已经预先运行一遍存在 `/process` 目录下了，如果后续有修改构建需要重新输出，直接查找这个标记取消相应注释就ok了

## 5 一些其他说明

==========

### 5.1 联系方式

zengyicydd@tju.edu.cn

gaocan@tju.edu.cn

daimingjing142857@tju.edu.cn

jsyy@tju.edu.cn
