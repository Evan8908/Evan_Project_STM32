# Evan_Project_STM32 项目说明

## 项目信息

- **项目用途**：嵌入式夹具程序开发（STM32）
- **本地路径**：`A:\Work_Files\Evan_Project_STM32`
- **GitHub 仓库**：`git@github.com:Evan8908/Evan_Project_STM32.git`
- **开发工具**：Keil MDK + VSCode

## Git 配置

- **用户名**：Evan
- **邮箱**：huahui453@gmail.com
- **GitHub 账号**：Evan8908
- **SSH 密钥**：ed25519，已配置到 GitHub
- **系统 SSH**：`git config --global core.sshCommand "C:/Windows/System32/OpenSSH/ssh.exe"`

## 项目结构

```
Evan_Project_STM32/
├── Fixture Program/        # 192点夹具 STM32 工程（Keil）
│   ├── CORE/               # 内核文件
│   ├── HARDWARE/           # 外设驱动（ADC/IIC/USART/PWM等）
│   ├── STM32F10x_FWLib/    # 标准外设库
│   ├── SYSTEM/             # 系统文件（delay/sys/usart）
│   └── USER/               # 用户代码（main.c/app.c等）
├── .gitignore              # 忽略Keil编译中间文件
├── CLAUDE.md               # 本文件
└── VSCode_GitHub操作说明书_v2.md  # Git操作参考文档
```

## 日常 Git 操作

```bash
# 开始工作前
git pull

# 写完代码后
git add .
git commit -m "feat/fix/chore: 说明改了什么"
git push
```

## 提交说明规范

| 类型 | 用途 |
|------|------|
| `feat:` | 新增功能 |
| `fix:` | 修复Bug |
| `refactor:` | 重构优化 |
| `chore:` | 配置杂项 |

## .gitignore 说明

已忽略以下文件（不上传到 GitHub）：
- Keil 编译中间文件：`*.crf` `*.dep` `*.lnp` `*.axf` `*.hex` 等
- Keil 个人设置：`*.uvoptx` `JLinkSettings.ini` `DebugConfig/`
- 系统文件：`Thumbs.db` `desktop.ini`

保留上传的重要文件：
- Keil 工程文件：`*.uvprojx`
- 所有源码：`*.c` `*.h` `*.s`
- 分散加载文件：`*.sct`

## 参考文档

详细操作步骤见：`VSCode_GitHub操作说明书_v2.md`
