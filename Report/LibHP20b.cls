\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{LibHP20b}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions
\LoadClass[12pt]{article}

\RequirePackage[T1]{fontenc}
\usepackage{setspace}
\RequirePackage{float}
\RequirePackage{color}
\RequirePackage{times}       % Use the Times-Roman Font
\RequirePackage[scaled=0.88]{inconsolata} % A nice monospaced font for listings
\RequirePackage{hyperref}    % URLs, etc.
\RequirePackage{graphicx}    % System for including images
\RequirePackage{listings}    % Nicely formatted C listings

\definecolor{darkgreen}{rgb}{0,0.3,0}
\definecolor{darkblue}{rgb}{0,0,0.3}
\definecolor{funccolor}{rgb}{0.6,0.25,0.1}
\definecolor{buttoncolor}{rgb}{0.0,0.25,0.05}
\definecolor{salmonpink}{rgb}{0.5,0.1,0.1}
\definecolor{charred}{rgb}{0.3,0.05,0.05}
\definecolor{darkgrey}{rgb}{0.1,0.1,0.1}

\newcommand{\Figure}[4]{
  \begin{figure}[#1]
    \centering
    \includegraphics[#2]{#3}
    \caption{#4}\label{fig:#3}
  \end{figure}
}

\lstset{
  language=C,
  tabsize=3,
  basicstyle={\ttfamily\small},
  keywordstyle=\color{blue},
  commentstyle=\color{darkgreen},
  stringstyle=\color{charred},
  numbers=left,                   % where to put the line-numbers
  numberstyle=\tiny\color{darkgrey},  % the style that is used for the line-numbers
  stepnumber=1,                   % the step between two line-numbers. If it's 1,
  morekeywords={true,false,byte,uint,ushort,uchar,Nybble,Number,llong,ullong,HP20bKey,HP20bKeys,HP20b_Calculator}
}

\lstnewenvironment{Code}
{\singlespacing}
{}


\setlength\pdfpageheight{\paperheight}
\setlength\pdfpagewidth{\paperwidth}

\def\figref#1{see Figure~\ref{fig:#1}}
\def\tabref#1{see Table~\ref{tab:#1}}
\def\secref#1{see Section~\ref{sec:#1}}

% Nicer looking section and subsection headers
\renewcommand\section{\@startsection {section}{1}{\z@}%
  {-0.8ex \@plus -0.8ex \@minus -.3ex}%
  {0.8ex \@plus .2ex}%
  {\normalfont\bfseries}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
  {-0.8ex \@plus -0.8ex \@minus -.3ex}%
  {0.8ex \@plus .2ex}%
  {\normalfont\itshape}}

\bibliographystyle{plain}