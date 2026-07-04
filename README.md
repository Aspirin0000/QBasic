<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.x-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Edition-Interpreter-8B5CF6?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Focus-Education-0EA5E9?style=for-the-badge" />
</p>

# MiniBasic (Qt Edition)

**基于 Qt Widgets 的 MiniBasic 解释器**，把教学语言执行链路从**词法 -> 语法 -> 执行 -> GUI 显示**串成完整闭环。

## 目录
- [项目亮点](#项目亮点)
- [目录与核心模块](#目录与核心模块)
- [构建方式](#构建方式)
- [清理与整理说明](#清理与整理说明)
- [后续优化](#后续优化)

## 项目亮点

- `tokenizer` + `parser` 形成基础编译器前端
- `program` + `statement` + `expression` 落地求值流程
- `evalstate` 管理运行状态与变量/求值上下文
- GUI 层提供可交互窗口（含程序输出、错误反馈与交互入口）

## 目录与核心模块

```text
MiniBasic.pro
├─ main.cpp / mainwindow.*      # Qt 启动与主界面
├─ parser.{cpp,h}               # 语句与表达式解析
├─ expression.{cpp,h}
├─ statement.{cpp,h}
├─ program.{cpp,h}
├─ tokenizer.{cpp,h}
├─ evalstate.{cpp,h}           # 执行状态
├─ myexceptions.h               # 错误模型
└─ checkIsValid.{cpp,h}         # 简易校验辅助
```

## 构建方式

```bash
git clone https://github.com/Aspirin0000/QBasic.git
cd QBasic
qmake MiniBasic.pro
make -j
./MiniBasic
```

使用 Qt Creator 时可直接打开 `MiniBasic.pro`。
建议构建目录隔离，避免污染源码树。

## 清理与整理说明

- 已清理：`QBasic-doc.docx`（课程汇报类文档，非代码依赖）
- 保留：全部解释器实现文件、项目文件、编译链路

## 开发命令

```bash
qmake MiniBasic.pro
make -j
make clean   # 强制清理构建中间产物
```

## 后续优化

- [ ] 增加最小回归样例集合（变量、循环、异常路径）
- [ ] 增加错误恢复与运行日志面板
- [ ] 统一示例输入脚本，降低首次上手成本

## 许可证说明

当前仓库暂未收录 SPDX 许可证文件；如对外分发建议补充正式 `LICENSE`。
