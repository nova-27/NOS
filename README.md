<h3 align="center">Nova27's Operating System</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> Nova27's Operating System は<br>
x86_64 UEFI環境で動くように設計されたオペレーティングシステムです
</p>

## 📝 目次

- [概要](#about)
- [必要なもの](#tools)
- [ビルド](#build)
- [テスト](#test)
- [作者](#author)

## 🧐 概要 <a name="about"></a>

C/C++/Assembly言語初心者がGUI等の実装を目指して開発しているOSです。<br>
Nova27's Operating System には、New OS、NOS is Open Source という意味も込められています（適当）

## 🧰 必要なもの <a name="tools"></a>

ソフトウェアのビルドにはこれらが必要です。

```
Linux(又はWindows Subsystem for Linux)
make
gcc-mingw-w64-x86-64
g++
lld
nasm
qemu-system-x86
最新のovmfバイナリ
```

## 🚧 ビルド <a name="build"></a>

`bootloader`と`kernel`ディレクトリで以下のコマンドを実行することでバイナリが作成されます。

```
make all
```

## 🚧 テスト <a name="test"></a>

最新のovmfバイナリをダウンロードし、`OVMF_CODE.fd`と`OVMF_VARS.fd`を`ovmf`フォルダに入れます。<br>
`bootloader`ディレクトリに生成された`main.efi`ファイルを`root`フォルダに移動します。<br>
その後`kernel`ディレクトリで以下のコマンドを実行することでQEmuが起動します。

```
make run
```

## ✍️ 作者 <a name="author"></a>

- [nova27](https://github.com/nova-27)



