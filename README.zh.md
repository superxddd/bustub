<img src="logo/bustub-whiteborder.svg" alt="BusTub 标志" height="200">

-----------------

[![构建状态](https://github.com/cmu-db/bustub/actions/workflows/cmake.yml/badge.svg)](https://github.com/cmu-db/bustub/actions/workflows/cmake.yml)

BusTub 是为卡内基梅隆大学 [数据库系统导论](https://15445.courses.cs.cmu.edu)（15-445/645）课程编写的关系型数据库管理系统。它仅用于教学，不适合生产环境。

BusTub 支持基础 SQL，并带有交互式 shell。完成所有课程项目后即可运行。

<img src="logo/sql.png" alt="BusTub SQL" width="400">

**警告：如果你是课程在读学生，不要直接 fork 本仓库，也不要公开提交项目答案。这会违反学术诚信，即使毕业后也可能被追责。**

我们会在 CMU 学生截止日期之后，将每次作业的自动评分器在 Gradescope 上对非 CMU 学生开放。作为交换，请不要在 GitHub 等公开代码库发布你的实现。请阅读课程 FAQ 了解如何在 Gradescope 使用自动评分器。提交前运行 `python3 gradescope_sign.py` 签署协议。

**警告：如果你不是 CMU 学生，请勿公开你的解答，并且务必提交自己的代码，否则会被封禁自动评分器。** 感谢大家维护公平的学习环境。

## 克隆本仓库

以下流程改编自 GitHub 关于[复制仓库](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-on-github/duplicating-a-repository)的文档，用来创建一个供你开发的私有 BusTub 仓库。

1. 在 GitHub 新建仓库（例如 `bustub-private`），可见性选择 **Private**。
2. 在本机创建公共仓库的裸克隆：
   ```console
   $ git clone --bare https://github.com/cmu-db/bustub.git bustub-public
   ```
3. 将公共仓库镜像推送到你的私库（假设 GitHub 用户名为 `student`，私库名 `bustub-private`）：
   ```console
   $ cd bustub-public
   
   # 如果用 HTTPS
   $ git push https://github.com/student/bustub-private.git master

   # 如果用 SSH
   $ git push git@github.com:student/bustub-private.git master
   ```
   复制完成后可以删除本地公共克隆：
   ```console
   $ cd ..
   $ rm -rf bustub-public
   ```
4. 克隆你的私库到开发机：
   ```console
   # HTTPS
   $ git clone https://github.com/student/bustub-private.git

   # SSH
   $ git clone git@github.com:student/bustub-private.git
   ```
5. 将公共仓库添加为第二个远端，便于随学期同步上游更新：
   ```console
   $ git remote add public https://github.com/cmu-db/bustub.git
   ```
   可用以下命令确认远端：
   ```console
   $ git remote -v
   origin  https://github.com/student/bustub-private.git (fetch)
   origin  https://github.com/student/bustub-private.git (push)
   public  https://github.com/cmu-db/bustub.git (fetch)
   public  https://github.com/cmu-db/bustub.git (push)
   ```
6. 需要时从公共仓库拉取更新：
   ```console
   $ git pull public master
   ```
7. 在私库设置中**关闭 GitHub Actions**，以免耗尽配额：
   ```
   Settings > Actions > General > Actions permissions > Disable actions.
   ```

建议在不同分支上完成各项目。如果不熟悉 Git 分支，请先学习；否则可能丢失全部进度且无人能帮你恢复。

## 构建

推荐在 Ubuntu 24.04 开发 BusTub，或使用 macOS（M1/M2/Intel，仅开发，不作为评分环境）。不支持其他环境（包括 WSL）。评分环境运行 Ubuntu 24.04。

### Linux（推荐）/ macOS（仅开发）

先运行脚本安装所需依赖：

```console
# Linux
$ sudo build_support/packages.sh
# macOS
$ build_support/packages.sh
```

然后编译：

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

如需 Debug 模式：

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -j`nproc`
```

Debug 默认启用 [AddressSanitizer](https://github.com/google/sanitizers)。

使用其他 Sanitizer 的示例：

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUSTUB_SANITIZER=thread ..
$ make -j`nproc`
```

macOS 与 Linux 在互斥量等行为上存在差异，可能导致测试结果不同。遇到问题时，优先在 Linux VM 复现。
