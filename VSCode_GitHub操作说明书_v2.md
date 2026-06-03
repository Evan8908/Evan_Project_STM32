# VSCode + GitHub 完整操作说明书 v2
适用于嵌入式开发 / STM32 / STC / ESP32 / Arduino

---

## 第一章 环境配置（一次性完成）

### 1.1 安装 Git

访问 https://git-scm.com/download/win 下载安装包。

安装时注意以下选项：
- 编辑器：选 Visual Studio Code
- PATH 设置：选 Git from the command line and also from 3rd-party software
- 换行符：选 Checkout Windows-style, commit Unix-style

安装完成后在终端验证：
```
git --version
```
显示版本号说明安装成功。

---

### 1.2 配置 Git 用户信息（必须完成）

```
git config --global user.name "你的名字"
git config --global user.email "你的邮箱@example.com"
```

> 邮箱必须和 GitHub 注册邮箱一致，否则提交记录不会关联到你的账号。

---

### 1.3 配置 Git 使用系统 SSH（Windows 必须）

Windows 上 Git 自带的 SSH 和系统 SSH 不共享配置，必须指定使用系统 SSH：

```
git config --global core.sshCommand "C:/Windows/System32/OpenSSH/ssh.exe"
```

---

### 1.4 生成 SSH 密钥

```
ssh-keygen -t ed25519 -C "你的邮箱@example.com"
```

三次提示全部直接回车（默认路径，不设密码）。

查看公钥内容：
```
cat ~/.ssh/id_ed25519.pub
```

---

### 1.5 将公钥添加到 GitHub

1. 登录 GitHub → 右上角头像 → Settings
2. 左侧菜单 → SSH and GPG keys
3. 点击 New SSH key
4. Title 填写：工作电脑
5. Key 粘贴公钥内容
6. 点击 Add SSH key

验证是否成功：
```
ssh -T git@github.com
```
显示 `Hi 用户名! You've successfully authenticated` 说明配置成功。

---

## 第二章 创建和克隆仓库

### 2.1 在 GitHub 创建新仓库

1. 登录 GitHub → 右上角 + → New repository
2. 填写仓库名（如 `Evan_Project_STM32`）
3. 选 Private（私有）
4. 勾选 Add a README file
5. .gitignore 选择 C 模板
6. 点击 Create repository

---

### 2.2 克隆仓库到本地

在 GitHub 仓库页面点绿色 Code 按钮 → 选 SSH 标签 → 复制地址

```
git clone git@github.com:你的用户名/仓库名.git "本地路径"
```

例：
```
git clone git@github.com:Evan8908/Evan_Project_STM32.git "A:\Work_Files\Evan_Project_STM32"
```

---

## 第三章 日常使用流程

### 核心概念

GitHub 就像一个云端硬盘，专门为代码设计：

```
你的电脑（本地）  ←→  GitHub（云端）
   写代码              备份 + 历史记录
```

三个动作：

| 动作 | 命令 | 比喻 |
|------|------|------|
| 选择要保存的文件 | `git add .` | 把文件放进信封 |
| 保存一个版本快照 | `git commit` | 把信封封好写上说明 |
| 上传到 GitHub | `git push` | 把信封寄出去 |

---

### 3.1 标准操作流程

**开始工作前，先拉取最新代码：**
```
git pull
```

**写完代码后，依次执行三条命令：**
```
git add .
git commit -m "feat: 说明你改了什么"
git push
```

> 不需要每改一行就提交，一个功能做完或一天结束时提交一次就够。

---

### 3.2 提交说明规范

| 类型 | 格式 | 示例 |
|------|------|------|
| 新功能 | `feat: 说明` | `feat: 添加SPI读写函数` |
| 修复Bug | `fix: 说明` | `fix: 修复UART初始化超时` |
| 重构优化 | `refactor: 说明` | `refactor: 重构ADC采样逻辑` |
| 文档更新 | `docs: 说明` | `docs: 更新README接线说明` |
| 杂项配置 | `chore: 说明` | `chore: 更新.gitignore` |

---

### 3.3 在 VSCode 界面操作（不用命令）

1. 按 `Ctrl+Shift+G` 打开源代码管理面板
2. 修改的文件会自动列出
3. 点文件右边的 `+` 暂存（等于 git add）
4. 顶部输入框写提交说明，点提交按钮（等于 git commit）
5. 点左下角同步更改按钮（等于 git push）

---

## 第四章 查看历史和回退代码

### 4.1 查看提交历史

```
git log --oneline
```

输出示例：
```
f17a825 chore: 移除Keil编译中间文件     ← 最新
a9fd130 feat: 导入Fixture Program工程
d7ebf9d chore: 添加.gitignore配置
```

---

### 4.2 回到之前的版本

**只是查看某个版本的代码（不修改）：**
```
git checkout a9fd130
```
看完后回到最新版本：
```
git checkout main
```

**真正回退到某个版本（放弃之后的改动）：**
```
git revert HEAD
```
这会生成一条新记录撤销最近一次提交，安全，不破坏历史。

> 前提：想回退的那个版本必须做过 git commit，否则 Git 不知道那时的状态。
> 所以每天结束时 commit 一次很重要。

---

## 第五章 .gitignore 配置（嵌入式项目）

`.gitignore` 告诉 Git 哪些文件不需要上传，避免上传编译产生的垃圾文件。

### STM32 / Keil 项目推荐配置

```gitignore
# 编译生成的中间文件
*.o
*.d
*.elf
*.bin
*.hex
*.map
*.lst
*.axf

# Keil 编译中间文件
*.crf
*.dep
*.lnp
*.iex
*.build_log.htm
*.htm
*.tra
*.sfd

# 编译输出目录
Build/
Debug/
Release/
Objects/
output/

# Keil 个人设置文件
*.uvguix.*
*.uvoptx
*.scvd
*.bak
JLinkSettings.ini
DebugConfig/

# STM32CubeIDE / Eclipse 配置文件
*.launch
.settings/
.project
.cproject

# VSCode 个人设置
.vscode/
*.code-workspace

# 系统文件
.DS_Store
Thumbs.db
desktop.ini
```

> 注意：`.uvprojx`（Keil 工程文件）和 `.sct`（分散加载文件）需要保留，不要忽略。

---

## 第六章 常见问题解决

### 推送被拒绝（rejected）

```
git pull --rebase origin main
git push
```

原因：远程有更新，先拉取再推送。

---

### 合并冲突（Conflict）

VSCode 会高亮显示冲突区域，选择保留哪段代码后：
```
git add .
git commit
```

---

### 访问 GitHub 速度慢

- 优先使用 SSH 方式（比 HTTPS 稳定）
- 如有代理工具：
```
git config --global http.proxy http://127.0.0.1:7890
```

---

## 第七章 常用命令速查

| 分类 | 命令 | 说明 |
|------|------|------|
| 状态查看 | `git status` | 查看哪些文件被修改 |
| 状态查看 | `git log --oneline` | 查看提交历史 |
| 日常操作 | `git add .` | 暂存所有改动 |
| 日常操作 | `git commit -m "说明"` | 提交 |
| 日常操作 | `git push` | 推送到 GitHub |
| 日常操作 | `git pull` | 拉取最新代码 |
| 回退操作 | `git checkout 提交ID` | 查看某个历史版本 |
| 回退操作 | `git checkout main` | 回到最新版本 |
| 回退操作 | `git revert HEAD` | 撤销最近一次提交 |
| 撤销操作 | `git reset --soft HEAD~1` | 撤销提交但保留代码改动 |
| 临时保存 | `git stash` | 临时保存未完成的改动 |
| 临时保存 | `git stash pop` | 恢复临时保存的改动 |

---

## 附录：你的项目配置信息

| 项目 | 内容 |
|------|------|
| Git 用户名 | Evan |
| Git 邮箱 | huahui453@gmail.com |
| GitHub 账号 | Evan8908 |
| 仓库地址 | git@github.com:Evan8908/Evan_Project_STM32.git |
| 本地路径 | A:\Work_Files\Evan_Project_STM32 |
| SSH 密钥类型 | ed25519 |
