# HHK-Bluetooth

## HHKのBluetooth化改造

既存のBluetooth modsは幾つかあるのですが、なんとなく自分で作ってみたくなったので検討してみました。

HHKBの内部構造とかキーマトリクスの情報は https://raw.github.com/tmk/tmk_keyboard/master/keyboard/hhkb/doc/HHKB.txt に詳しく載っています。

これから考えると、

HHKをBluetooth化するために必要なコントローラの要件は

* HIDプロトコルをサポートしたBluetoothインターフェイスを持っていること
 * 技適のことを考えると、USBドングルを使えることが望ましい
* 5V系、8ポートの出力ポートと、1ポートの入力ポート
* 低消費電力であること

です。

いろいろと(主に脳内で)検討してみたのですが、
http://phys.sci.hokudai.ac.jp/LABS/yts/pic/GB002/GB002.html
の記事が決め手となって、PIC24FJ64GB002で作ってみようと思いました。

[方針](Documents/Concept.md)

[計画](Documents/Plam.md)
