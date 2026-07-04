# MiniBasic (Qt Edition)

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.x-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Project-Educational-0EA5E9?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Style-Interpreter-8B5CF6?style=for-the-badge" />
</p>

> MiniBasic：Qt Widgets 下的 BASIC 方言解释器，实现从词法、语法到执行的完整教学链路。

## ⚡ 核心能力

- ✅ `tokenizer` → `parser` → `program` 三层流水线
- ✅ 语句求值与运行时状态管理（`evalstate`）
- ✅ GUI 调试窗体（`MainWindow`）
- ✅ 可视化错误展示与交互入口

## 🧱 项目结构

```text
MiniBasic.pro
.
├─ main.cpp / mainwindow.*      # Qt 启动与主界面
├─ parser.{cpp,h}               # 词法/语法分析
├─ expression.{cpp,h}
├─ statement.{cpp,h}
├─ program.{cpp,h}
├─ tokenizer.{cpp,h}
├─ evalstate.{cpp,h}
└─ myexceptions.h                # 异常定义
```

## 🧪 编译运行

```bash
git clone https://github.com/Aspirin0000/QBasic.git
cd QBasic
qmake MiniBasic.pro
make -j
./MiniBasic
```

如你使用 Qt Creator，可直接打开 `MiniBasic.pro`。建议在源码目录外创建构建目录以避免污染。

## 🧼 文件整理

- 已清理：Qt 临时输出、`.user` 及本地构建目录。
- 已保留：核心解释器源码、编译脚本与教学相关代码。
- 删除的文档：`QBasic-doc.docx`（课程提交快照，非源码依赖）。

## 🛠️ 开发命令

```bash
qmake MiniBasic.pro
make -j
# debug
./MiniBasic
```

```bash
# 强制清理（如需）
make clean
```

## 🛣️ 下一步

- [ ] 增加最小回归测试集
- [ ] 增加更完整脚本示例和错误恢复演示
- [ ] 为解释器加上运行日志面板

## 🛡️ License

教育性质项目。当前仓库未内置 `LICENSE`，如需对外发布建议补充许可文件。
