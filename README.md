iPuTTY Bitbucket.org Repository
==

이 repository는 2012년 부터 2016년까지 bitbucket.org의 project로 유지되었던 mercurial repository 보존용 입니다.


## Welcome

iPuTTY is a Korean localized version of PuTTY, the SSH + terminal emulator. The main target platform of iPuTTY is Windows.

## NOTICE (2017/05/14)

Now, as a new maintainer, [JoungKyun Kim](http://oops.org|JoungKyun Kim) is planning to lead the development of iPuTTY project.

This project is currently migrating to GitHUB. The new address is https://github.com/iPuTTY/iPuTTY.
Current, version 0.69 is ready to release and will be released when the site transfer is completed.

## NOTICE (2016/05/31)

This project is no longer actively maintained.
Since this is an open-source project, feel free to fork it and make advances!

If you need latest updates including iPuTTY's additions, take a look at follows:
  * [HPuTTY](https://github.com/teamnop/HPuTTY) : PuTTYTray + iPuTTY + @
  * [iPuTTY(Cygterm)](https://github.com/joungkyun/iputty) : iPuTTY + Cygterm + 한글 UI + @

Please let me know if you have another fork that includes iPuTTY.

### Key Additions

 * **On-the-spot IME support** - The composition of Hangul syllables happens directly at the cursor. No more ugly gray boxes for IME composition randomly positioned outside the terminal.
 * **CP949/UTF-8 quick switching menu** - It provides a quick-fix for legacy Linux servers.
 * **Separate ANSI/Unicode fonts** - You can use Consolas for ANSI characters and 돋움 for Unicode characters including Hangul syllables in the same terminal.
 * **Semi-transparent windows**
   * Transparency can be configured.
   * ```Alt+[```, ```Alt+]``` for quick-change of transparency (among almost transparent, semi-transparent, and completely opaque) - It is great to see webpages and documents //through// the terminal when you do not have multiple monitors.
   * ```Alt+Shift+[```, ```Alt+Shift+]``` for fine adjustment of transparency
 * **New keyboard shortcuts**
   * ```Ctrl+Tab``` to switch to other PuTTY windows.
 * **Version-aware default fonts on Windows** (Consolas for Windows 7 and Vista, Courier New for Windows XP, for example)
 * Now, **UTF-8 is the default encoding** if not configured explicitly. This will be a huge convenience since most Linux serve
rs uses it by default recently.
 * ***Italics font*** support: To use it on xterm-256color terminals, have a look at [my terminfo generator](https://bitbucket.org/daybreaker/snippets/src/tip/home-config/gen-italics-terminfo.sh?at=default) and related configurations such as vimrc, bashrc, and tmux.conf in the same location.

## 한글판에 대한 공지

현재 PuTTY가 Unicode 응용프로그램으로 개발되지 않은 상태이기 때문에 내부적으로 ANSI API를 사용합니다. 따라서 한글UI를 적용할
경우 한국어 Windows가 아닌 환경에서는 깨져보일 수 있습니다. 우선 영문판으로 기능적인 부분을 모두 구현한 후, UI에 한글 적용한
버전은 추후 별도 branch로 관리할 예정이오니 참고 바랍니다.


## 오래된 Windows 버전에 대한 지원

기존 한글 iPuTTY의 경우 Windows 98 환경에서도 돌아가게 만드는 workaround를 적용하기도 했었지만, 기본적으로 원본 PuTTY와 동일하게
OS 지원을 할 예정입니다. (예를 들면 0.61에서 클래식 Mac 운영체제에 대한 지원이 중단되었습니다.) 개발 버전은 최신 윈도 환경을
가정하고 릴리즈되며, 정식 릴리즈 버전은 필요에 따라 클래식 버전을 따로 나누는 것을 고려하고 있습니다.
