<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.x-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Edition-Interpreter-8B5CF6?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Focus-Education-0EA5E9?style=for-the-badge" />
</p>

# MiniBasic (Qt Edition)

一个完整链路的 MiniBasic 教学解释器：**词法 → 语法 → 执行 → GUI 展示**，用于课程演示、解释器学习和桌面交互式调试的最小闭环。

> 重点不是“功能更多”，而是把编译器基础流程做成能运行、能展示、易扩展的一条通路。

---

- [项目价值](#项目价值)
- [核心流水线](#核心流水线)
- [功能清单（按模块）](#功能清单按模块)
- [目录组织](#目录组织)
- [快速运行](#快速运行)
- [可交付价值](#可交付价值)
- [优化方向](#优化方向)
- [许可说明](#许可说明)

## 项目价值

- **课程展示价值**：清晰呈现“MiniBasic 方言解释器”完整生命周期。
- **工程价值**：将解析器、语义执行、状态管理和 GUI 组织到独立模块，便于后续替换 `Lexer/Parser/Executor` 实现。
- **项目包装价值**：适合作为个人主页作品的“非纯炫目但工程可读”的代表项目，突出问题解决能力。

## 核心流水线

```text
源代码输入
   ↓
Tokenizer（词法）
   ↓
Parser（语法树/语句）
   ↓
Statement + Expression（执行语义）
   ↓
Program / EvalState（运行时状态）
   ↓
GUI（MainWindow）展示输出
```

## 功能清单（按模块）

- `tokenizer.*`：源代码切词、关键字与符号识别。
- `parser.*`：语句分发与结构化解释。
- `expression.*`、`statement.*`：表达式与语句执行策略。
- `program.*`：程序体组织与逐步执行。
- `evalstate.*`：运行时环境（变量表/执行状态）。
- `mainwindow.*` + `MiniBasic.pro`：Qt 启动、交互与错误反馈窗口。

## 目录组织

```text
MiniBasic.pro
├─ main.cpp / mainwindow.*          # 启动入口与主界面
├─ tokenizer.{cpp,h}                # 词法层
├─ parser.{cpp,h}                   # 语法层
├─ expression.{cpp,h}               # 表达式求值
├─ statement.{cpp,h}                # 语句执行单元
├─ program.{cpp,h}                  # 程序组织
├─ evalstate.{cpp,h}                # 执行环境
├─ myexceptions.h                   # 错误与异常模型
└─ checkIsValid.{cpp,h}             # 输入校验辅助
```

## 快速运行

```bash
git clone https://github.com/Aspirin0000/QBasic.git
cd QBasic
qmake MiniBasic.pro
make -j
./MiniBasic
```

使用 Qt Creator 可直接打开 `MiniBasic.pro` 构建。

## 可交付价值（你能展示什么）

可以在 README 或演示中直接说：

- 我把解释器分成“前端解析层、执行层、GUI 层”三层，说明完整软件设计边界。
- 我保留了最小可运行链路，不依赖外部服务，便于评审和二次实验。
- 我移除了课程阶段非必要文档后，仅保留能复用的核心源码和构建入口。

示例交互输入：

```text
LET A = 10
PRINT A + 5
IF A > 5 THEN PRINT "ok"
```

## 优化方向

- [ ] 增加回归测试文件（关键表达式、条件分支、错误恢复）
- [ ] 做一套统一示例脚本，开箱即可演示核心能力
- [ ] 增强错误面板，支持运行日志与断点式观察

## 许可说明

当前仓库暂未提供标准 SPDX `LICENSE` 文件；若对外分发，建议补充正式许可文本。
