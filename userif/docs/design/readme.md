# setup doxygen
1. install tex-live  
`sudo apt install texlive-lang-japanese`  
`sudo apt install texlive-xetex`  

1. install doxygen  
`sudo apt install doxygen`  

# usage
1. `CustomDoxyfile`を編集(PROJECT_NAME, INPUT, STRIP_FROM_PATHなど)  
1. `custom_info.sty`を編集  
1. `./makepdf.sh <filename>`でPDF作成（<filename>を指定しない場合、refman.pdfとなる）  

`html`にも出力される  
`custom_header.tex, custom_footer.tex`を修正して出力内容を変更可  
documentに含めたいソースコード(cファイル, hファイル)は先頭行に以下を記入する  
```
/// \file  
/// \brief  
```