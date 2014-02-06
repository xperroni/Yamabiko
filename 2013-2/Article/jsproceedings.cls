%
% pLaTeX2e 学術会議 Proceedings 用クラスファイル
%
% Last modified on 2013-06-11
% by 原 祥尭 (HARA, Yoshitaka) <hara.y@roboken.esys.tsukuba.ac.jp>
%
\typeout{**** Document Class for Proceedings by hara.y ****}

% 新ドキュメントクラス
% http://oku.edu.mie-u.ac.jp/~okumura/jsclasses/
\LoadClassWithOptions{jsarticle}


%% パッケージ

% 図表
\usepackage[dvipdfmx]{graphicx}  % 図を挿入する \includegraphics コマンド
% \usepackage{subfigure}  % figure 環境に複数の図を挿入するための \subfigure コマンド（CTAN の obsolete 以下にあるので非推奨）
\usepackage[caption=false,font=footnotesize,labelformat=simple]{subfig}  % \subfigure コマンドの改善版の \subfloat コマンド
% \usepackage{hangcaption}  % キャプションにインデントを付けたり幅を指定可能な \hangcaption コマンド
\usepackage{booktabs}  % 表の横罫線の改善版コマンド（\toprule, \midrule, \cmidrule, \bottomrule）
\usepackage{hhline}  % 表の二重罫線を拡張
\usepackage{multirow}  % 表の縦二段をつなげる \multirow コマンド
\usepackage{tabularx}  % 横幅を指定可能な表の tabularx 環境
\usepackage{eqparbox}  % 自動的に同じ幅に調整されるボックス（表など）の \eq­par­box コマンド
% \usepackage{float}  % \begin{figure}[H] で強制的にその位置に図表を出力、新しいフロート環境を定義する \newfloat コマンド、here.sty の改善版
\usepackage{fixltx2e}  % 1カラムと2カラムの図の順番が入れ替わるバグなどを修正
\usepackage{array}  % 標準の array 環境と tabular 環境を改善

% アルゴリズム
% http://en.wikibooks.org/wiki/LaTeX/Algorithms
% \if0  % algorithm.sty による algorithm 環境
% http://www.ctan.org/tex-archive/macros/latex/contrib/algorithmicx/
% http://tex.stackexchange.com/questions/29429/how-to-use-algorithmicx-package
% http://tex.stackexchange.com/questions/96587/algorithm-with-latex-package-algorithmicx
\usepackage{algorithm}  % algorithmic.sty（旧版）または algorithmicx.sty（新版）と共に使用する
\usepackage{algpseudocode}  % algorithmic 環境の改善版である algorithmicx.sty を使用（\usepackage{algorithmicx} のようには使わないので注意）
\newcommand{\algname}{\ALG@name~}  % ユーザ定義の参照コマンドで使用
% \fi
\if0  % algorithm2e.sty による algorithm 環境
% http://www.ctan.org/tex-archive/macros/latex/contrib/algorithm2e/
% http://tex.stackexchange.com/questions/41012/change-the-algorithm-enumeration-in-the-algorithm2e-package
\usepackage{algorithm2e}
\newcommand{\algname}{\algorithmcfname~}  % ユーザ定義の参照コマンドで使用
\fi

% 数式
% amsmath.sty を txfonts.sty の後で読み込むと command already defined エラーになるので注意
\usepackage[cmex10,fleqn]{amsmath}  % AMS の数式環境（amstext.sty, amsopn.sty, amsbsy.sty を含む）、cmex10 オプションでビットマップフォントの埋め込みを防ぐ
\usepackage{amssymb}  % AMSFonts（数学記号）を使用
\usepackage{amsthm}  % 定理型の環境を定義する \newtheorem コマンドを拡張、証明を書く proof 環境
% \usepackage{cases}  % 数式の場合分けそれぞれに行番号を付ける

% フォント
% http://tex.stackexchange.com/questions/664/why-should-i-use-usepackaget1fontenc
\usepackage[T1]{fontenc}  % アクセント記号付きの文字をハイフン連結可能にする、"|" "<" ">" を正しく表示する
% http://tex.stackexchange.com/questions/1291/why-are-bitmap-fonts-used-automatically
\usepackage{lmodern}  % T1 encoding でビットマップフォントになることを防ぐ
\usepackage{textcomp}  % TS1 encoding の記号
\if0  % デフォルトの Computer Modern でなく Times フォント（txfonts.sty）を使用
\usepackage{txfonts}  % 本文・数式を Times フォントにする
\fi
% \if0  % デフォルトの Computer Modern でなく Times フォント（mathptmx.sty + boldmath と \mathcal は txfonts.sty）を使用
% \usepackage{mathptm}  % 本文・数式を Times フォントにする旧版
\usepackage{mathptmx}  % 本文・数式を Times フォントにする新版
% boldmath がフォントを二重にしただけの太字で汚くなるのを防ぐために txfonts.sty を使用
% http://www-is.amp.i.kyoto-u.ac.jp/lab/kmaeda/latex/
\SetSymbolFont{operators}{bold}{OT1}{txr}{bx}{n}
\SetSymbolFont{letters}{bold}{OML}{txmi}{bx}{it}
\SetSymbolFont{symbols}{bold}{OMS}{txsy}{bx}{n}
\SetSymbolFont{largesymbols}{bold}{OMX}{txex}{bx}{n}
% \mathcal（筆記体）は mathptmx.sty の RSFS フォントが見難いので別のフォントを使用
% http://www.latex-cmd.com/style/calligraphy.html
% \DeclareMathAlphabet{\mathcal}{OMS}{cmsy}{m}{n}  % デフォルト（Computer Modern）の Calligraphic フォント
\DeclareMathAlphabet{\mathcal}{OMS}{txsy}{m}{n}  % txfonts.sty の Calligraphic フォント
% \fi
% http://tex.stackexchange.com/questions/3238/bm-package-versus-boldsymbol
\usepackage{bm}  % 数式用ボールドフォントの \bm コマンド（amsbsy.sty の改善版）、txfonts.sty や mathptmx.sty の前で読み込むと boldmath が Computer Modern になってしまうので注意
% \usepackage{pifont}  % 丸囲み1などの特殊記号（\ding{文字コード} コマンド）

% 体裁、各種コマンド
\usepackage{cite}  % 参考文献の参照を "[2,3,1]" から "[1--3]" または "[2], [3], [1]" から "[1]--[3]" に修正、英文での参照 "[1]" の前に空白がない場合に空白を挿入（和文では元々挿入される）
\usepackage{flushend}  % 最終ページの2カラムの左右の高さを揃える
\usepackage{url}  % URL を記載する \url コマンド
\usepackage{enumerate}  % 箇条書きの形式を個別に指定可能に拡張
\usepackage{comment}  % 複数行をコメントアウトする \begin{comment}, \end{comment}
\usepackage{layout}  % レイアウトを表示する \layout コマンド

% 下線
% http://oku.edu.mie-u.ac.jp/~okumura/texwiki/?TeX%E3%81%8C%E8%8B%A6%E6%89%8B%E3%81%A8%E3%81%99%E3%82%8B%E5%87%A6%E7%90%86#t65559ac
% \usepackage{jumoline}  % 下線コマンド（\Underline）、中線（取り消し線）コマンド（\Midline）、上線コマンド（\Overline）
% \usepackage{udline}  % 下線コマンド（\unl）、中線（取り消し線）コマンド（\sol）、上線コマンド（\ovl）など


%% ユーザ定義コマンド

% 所属の†1形式の脚注マーク
\newcommand{\authorrefmark}[1]{%
\begingroup%  設定変更のスコープ
\def\thefootnote{~\dag\arabic{footnote}~}%
\footnotemark[#1]\hspace*{1em}%
\endgroup}

% 所属の†1形式の脚注テキスト
\newcommand{\authorreftext}[2]{%
\begingroup%  設定変更のスコープ
\def\thefootnote{~\dag\arabic{footnote}~}%
\footnotetext[#1]{#2}%
\endgroup}

% 英語所属の‡1形式の脚注マーク
\newcommand{\eauthorrefmark}[1]{%
\begingroup%  設定変更のスコープ
\def\thefootnote{~\ddag\arabic{footnote}~}%
\footnotemark[#1]\hspace*{1em}%
\endgroup}

% 英語所属の‡1形式の脚注テキスト
\newcommand{\eauthorreftext}[2]{%
\begingroup%  設定変更のスコープ
\def\thefootnote{~\ddag\arabic{footnote}~}%
\footnotetext[#1]{#2}%
\endgroup}

% 図表の参照
\newcommand{\figref}{\@figref@}
\def\@figref@#1{\@ifundefined{figrefdef#1}{\textbf{\figurename\ref{#1}}}{\figurename\ref{#1}}%
\global\@namedef{figrefdef#1}{figrefdef#1}}
\newcommand{\tableref}{\@tableref@}
\def\@tableref@#1{\@ifundefined{tablerefdef#1}{\textbf{\tablename\ref{#1}}}{\tablename\ref{#1}}%
\global\@namedef{tablerefdef#1}{tablerefdef#1}}
% \newcommand{\figref}[1]{Fig.~\ref{#1}}
% \newcommand{\tableref}[1]{Table~\ref{#1}}
% \newcommand{\figref}[1]{\textbf{Fig.~\ref{#1}}}
% \newcommand{\tableref}[1]{\textbf{Table~\ref{#1}}}

% アルゴリズムの参照（\algref は algorithmicx.sty で定義されているため \algrefx を使用）
\newcommand{\algrefx}{\@algrefx@}
\def\@algrefx@#1{\@ifundefined{algrefdef#1}{\textbf{\algname\ref{#1}}}{\algname\ref{#1}}%
\global\@namedef{algrefdef#1}{algrefdef#1}}

% 数式での argmax、argmin
\newcommand{\argmax}{\mathop{\textrm{argmax}}\limits}
\newcommand{\argmin}{\mathop{\textrm{argmin}}\limits}

% 数式での絶対値、ノルム
\newcommand{\lrabs}[1]{\left\lvert#1\right\rvert}
\newcommand{\abs}[1]{\lvert#1\rvert}
\newcommand{\babs}[1]{\bigl\lvert#1\bigr\rvert}
\newcommand{\Babs}[1]{\Bigl\lvert#1\Bigr\rvert}
\newcommand{\bgabs}[1]{\biggl\lvert#1\biggr\rvert}
\newcommand{\Bgabs}[1]{\Biggl\lvert#1\Biggr\rvert}
\newcommand{\lrnorm}[1]{\left\lVert#1\right\rVert}
\newcommand{\norm}[1]{\lVert#1\rVert}
\newcommand{\bnorm}[1]{\bigl\lVert#1\bigr\rVert}
\newcommand{\Bnorm}[1]{\Bigl\lVert#1\Bigr\rVert}
\newcommand{\bgnorm}[1]{\biggl\lVert#1\biggr\rVert}
\newcommand{\Bgnorm}[1]{\Biggl\lVert#1\Biggr\rVert}

% ローマ数字
\newcommand{\roma}[1]{\expandafter{\romannumeral#1}}
\newcommand{\Roma}[1]{\uppercase\expandafter{\romannumeral#1}}

% 斜線表記の分数（solidus 記号を用いた diagonal fraction）
\def\sfrac#1#2{\leavevmode\kern-.1em
  \raise.5ex\hbox{\the\scriptfont0 #1}\kern-.1em
  /\kern-.15em\lower.25ex\hbox{\the\scriptfont0 #2}}


%% 体裁

% ページレイアウト（A4：297 mm × 210 mm、1 インチ = 25.4 mm）
% http://www.biwako.shiga-u.ac.jp/sensei/kumazawa/tex/layout.html
% 上 20 mm、下 22 mm、左右 20 mm の余白設定

\setlength{\topmargin}{-20truemm}
\setlength{\headheight}{10truemm}
\setlength{\headsep}{4.6truemm}
\setlength{\textheight}{255truemm}
\setlength{\oddsidemargin}{-5.4truemm}
\setlength{\evensidemargin}{-5.4truemm}  % twoside オプション指定時のみ有効
\setlength{\textwidth}{170truemm}
% \setlength{\columnsep}{8truemm}
\setlength{\columnsep}{6truemm}
\setlength{\paperwidth}{210truemm}
\setlength{\paperheight}{297truemm}

% 行間
% \baselineskip は \begin{document} の後に設定しないとリセットされる（ここで設定しても無効）ので \baselinestretch で設定するのが良い
% \setlength{\baselineskip}{3.33ex}  % 文字 x の高さ基準で相対的に指定
% \renewcommand{\baselinestretch}{0.95}  % 行間の相対的な倍率

% 日付リセット
\date{}

% ページ番号なし
\pagestyle{empty}

% 奇数ページの右上・偶数ページの左上に任意のヘッダ、ページ番号なし
% 奇数ページと偶数ページで異なるヘッダを用いるには \documentclass[twoside]{proceedings} とする必要あり（twoside オプション指定なしの場合は常に奇数ページの設定が使われる）
\def\ps@myheadings{%
%   \def\@oddhead{\hbox{}\hfil\rightmark}%
  \def\@oddhead{\hbox to \textwidth{\hfil\rightmark}}%
  \def\@oddfoot{}%
%   \def\@evenhead{\leftmark\hfil\hbox{}}%
  \def\@evenhead{\hbox to \textwidth{\leftmark\hfil}}%
  \let\@evenfoot\@oddfoot
  \let\@mkboth\@gobbletwo
  \let\sectionmark\@gobble
  \let\subsectionmark\@gobble
%   \def\sectionmark##1{}%
%   \def\subsectionmark##1{}%
}

% 奇数ページの右上・偶数ページの左上に任意のヘッダ、フッタにページ番号
% 奇数ページと偶数ページで異なるヘッダを用いるには \documentclass[twoside]{proceedings} とする必要あり（twoside オプション指定なしの場合は常に奇数ページの設定が使われる）
\def\ps@myheadingsplain{%
%   \def\@oddhead{\hbox{}\hfil\rightmark}%
  \def\@oddhead{\hbox to \textwidth{\hfil\rightmark}}%
  \def\@oddfoot{\normalfont\hfil\thepage\hfil}%
%   \def\@evenhead{\leftmark\hfil\hbox{}}%
  \def\@evenhead{\hbox to \textwidth{\leftmark\hfil}}%
  \let\@evenfoot\@oddfoot
  \let\@mkboth\@gobbletwo
  \let\sectionmark\@gobble
  \let\subsectionmark\@gobble
}

% タイトル
\def\title#1{\gdef\@title{#1}}
\def\author#1{\gdef\@author{#1}}
\def\etitle#1{\gdef\@etitle{#1}}
\def\eauthor#1{\gdef\@eauthor{#1}}
\def\affiliation#1{\gdef\@affiliation{#1}}
\long\def\abstract#1{\long\gdef\@abstract{#1}}
\def\keywords#1{\gdef\@keywords{#1}}
\def\@maketitle{\newpage%
  \null%
  \begin{center}%
%     \vspace*{-10mm}
    \vspace*{-5mm}
    {\LARGE\textbf{\@title}\par}%
    \vskip 1em
    {\large \@author \par}%
    \@ifundefined{@etitle}{}{%
%     \vskip 1em
    \vskip 0.5em
    {\Large\textbf{\@etitle}\par}%
    \gdef\@etitle{}}%
    \@ifundefined{@eauthor}{}{%
    \vskip 0.5em
    {\large \@eauthor \par}%
    \gdef\@eauthor{}}%
    \@ifundefined{@affiliation}{}{%
    \vskip 0.5em
    {\large \@affiliation \par}
    \gdef\@affiliation{}}%
%     \vskip 1em
    \vskip 0.5em
  \end{center}%
  \@ifundefined{@abstract}{}{%
    \small%
    \begin{center}%
      \begin{minipage}{0.9\textwidth}%
      {\textbf{Abstract---}}\@abstract%
      \end{minipage}%
    \end{center}%
    \gdef\@abstract{}}%
  \@ifundefined{@keywords}{}{%
    \small%
    \begin{center}
      \begin{minipage}{0.9\textwidth}%
      {\textbf{Keywords: }}\@keywords%
      \end{minipage}
    \end{center}
    \gdef\@keywords{}}%
%   \hbox{\kern 0.2\textwidth\vbox{\hrule width 0.6\textwidth}}%
  \vspace*{1mm}
}

% セクション名の末尾にピリオドを付けて "1." に変更する
% \renewcommand{\thesection}{\presectionname\@arabic\c@section.\postsectionname}  % \ref での参照にもピリオドが付いてしまう
% \renewcommand{\postsectionname}{.}  % \ref での参照にもピリオドが付いてしまう
% \def\@seccntformat#1{\csname the#1\endcsname.\quad}  % subsection などのすべての末尾にピリオドが付いてしまう
% http://www.h4.dion.ne.jp/~latexcat/column/column2.html
% http://www.math.tohoku.ac.jp/~atsushi/Tex/Mysect/
\def\@seccntformat#1{%  #1 はカウンタ名
   \@nameuse{@seccnt@prefix@#1}%
   \@nameuse{the#1}%
   \@nameuse{@seccnt@postfix@#1}%
   \quad}
\def\@seccnt@prefix@section{}
\def\@seccnt@postfix@section{.}
\def\@seccnt@prefix@subsection{}
\def\@seccnt@postfix@subsection{}
\def\@seccnt@prefix@subsubsection{}
\def\@seccnt@postfix@subsubsection{}

% jsarticle のセクション見出しフォントをボールド体にする
\renewcommand{\headfont}{\bfseries}

% セクションの体裁
% http://www.nsknet.or.jp/~tony/TeX/faq/macro.htm
% http://osksn2.hep.sci.osaka-u.ac.jp/~naga/miscellaneous/tex/tex-tips1.html
\if@twocolumn
  \renewcommand{\section}%  カウンタ名
    {\@startsection{section}%  #1 見出し
    {1}%  #2 見出しのレベル
    {\z@}%  #3 見出し左の空き（インデント量）
    {0.4\Cvs}%  #4 見出し上の空き
    {0.2\Cvs}%  #5 見出し下の空き（負の値なら改行せず見出し右の空き）
    {\normalfont\large\headfont\raggedright}%  #6 見出しのスタイル
  }
  \renewcommand{\subsection}%  カウンタ名
    {\@startsection{subsection}%  #1 見出し
    {2}%  #2 見出しのレベル
    {\z@}%  #3 見出し左の空き（インデント量）
    {0.2\Cvs}%  #4 見出し上の空き
    {\z@}%  #5 見出し下の空き（負の値なら改行せず見出し右の空き）
    {\normalfont\normalsize\headfont}%  #6 見出しのスタイル
  }
\fi

% センタリングしたセクションの体裁
\if@twocolumn
%   \newcommand{\csection}[1]{{\centering\section{#1}}}
  \newcommand{\csection}%  カウンタ名
    {\@startsection{section}%  #1 見出し
    {1}%  #2 見出しのレベル
    {\z@}%  #3 見出し左の空き（インデント量）
    {0.4\Cvs}%  #4 見出し上の空き
    {0.2\Cvs}%  #5 見出し下の空き（負の値なら改行せず見出し右の空き）
%     {\normalfont\large\headfont\raggedright}%  #6 見出しのスタイル
    {\normalfont\large\headfont\centering}%  中央揃え
%     {\normalfont\large\headfont\raggedleft}%  右揃え
  }
\fi

% 図表を記載した位置の近くに出力するための float パラメータ設定
\renewcommand{\topfraction}{1.0}  % ページ上部の図表が占める上限
\renewcommand{\dbltopfraction}{1.0}  % ページ上部の図表が占める上限（2カラムの*版）
\renewcommand{\bottomfraction}{1.0}  % ページ下部の図表が占める上限
\renewcommand{\textfraction}{0.0}  % ページの文章が占める割合の下限
\renewcommand{\floatpagefraction}{0.9}  % 出力位置 p が実行されるのに必要な最低限の図表占有比率
\renewcommand{\dblfloatpagefraction}{0.9}  % 出力位置 p が実行されるのに必要な最低限の図表占有比率（2カラムの*版）
\setcounter{topnumber}{10}  % ページ上部の図表の上限数
\setcounter{dbltopnumber}{20}  % ページ上部の図表の上限数（2カラムの*版）
\setcounter{bottomnumber}{10}  % ページ下部の図表の上限数
\setcounter{totalnumber}{20}  % ページあたりの図表の上限数

% figure や table のキャプションの体裁
\setlength\abovecaptionskip{\z@}  % キャプション上の空き（縦）
\setlength\belowcaptionskip{4\p@}  % キャプション下の空き（縦）

\if0
% subfigure の体裁（subfigure.sty を使用する場合）
\renewcommand{\subfigtopskip}{\z@}
\renewcommand{\subfigcapskip}{-6\p@}  % 図と subfigure キャプションの空き（縦）
\renewcommand{\subfigcaptopadj}{\z@}
\renewcommand{\subfigbottomskip}{\z@}  % subfigure キャプションと figure キャプションの空き（縦）
\renewcommand{\subfigcapmargin}{\z@}
\renewcommand{\subfiglabelskip}{4\p@}  % subfigure キャプションの "(a)" と文字列の空き（横）
\fi

% subfloat の体裁（subfig.sty を使用する場合）
% http://www.ctan.org/tex-archive/macros/latex/contrib/subfig/
% 以下のように設定せずに \usepackage[farskip=4pt,nearskip=4pt]{subfig} としても同じ設定が可能
\sf@farskip 4\p@  % 図と subfloat キャプションの集合に対して main float（figure など）のキャプションと反対側の空き（縦）
\sf@nearskip 4\p@  % 図と subfloat キャプションの集合に対して main float（figure など）のキャプションとの間の空き（縦）

% subfloat の \ref での参照を "1a" から "1(a)" に変更する（subfigure.sty では元々そうなる）
% http://tex.stackexchange.com/questions/16291/adding-parentheses-around-subfig-references
% 以下の設定と併せて \usepackage[labelformat=simple]{subfig} としている（subfloat キャプションが "((a))" になってしまう対策）
% subfloat キャプションの "(a)" のみを参照する場合は \subref* コマンドを使用（\subref コマンドでは "((a))" になってしまう）
\renewcommand{\thesubfigure}{(\alph{subfigure})}

% 図表のキャプション
\renewcommand{\figurename}{Fig.~}
\renewcommand{\tablename}{Table~}
\def\fnum@figure{\headfont\figurename\nobreak\thefigure\normalfont}
\def\fnum@table{\headfont\tablename\nobreak\thetable\normalfont}

% 文間空白と単語間空白を同じ幅（単語間空白）に揃える
% http://hwb.ecc.u-tokyo.ac.jp/current/applications/latex/alphlang/
% 略語のピリオド（"Proc." "Vol." "No." "pp." など）の後が文間空白になったり大文字の後のピリオド（や疑問符・感嘆符）の後が単語間空白になることを防ぐ
% 以下の設定をしない場合は、改行許容の単語間空白 "\ "、改行抑制の単語間空白 "~"、終止符指定（文間空白）"\@. " または "\null. " で空白を明示的に指定する必要がある
\frenchspacing

% 数式途中での改ページを許可する
% http://www-is.amp.i.kyoto-u.ac.jp/lab/kmaeda/latex/
\allowdisplaybreaks

% 箇条書きの形式を "1." から "1)" に変更する
% http://keizai.xrea.jp/latex/tutorial/list.html
\renewcommand{\labelenumi}{\theenumi)}
\renewcommand{\labelenumii}{\theenumii)}
\renewcommand{\labelenumiii}{\theenumiii)}
\renewcommand{\labelenumiv}{\theenumiv)}

% 脚注の罫線を段の幅と等しくする
\renewcommand{\footnoterule}{%
  \kern -3\p@
  \hrule width \columnwidth
  \kern 2.6\p@
}

% 脚注の形式を "*1" に変更する
% http://www.latex-cmd.com/struct/footnote.html
\renewcommand{\thefootnote}{*\arabic{footnote}}
