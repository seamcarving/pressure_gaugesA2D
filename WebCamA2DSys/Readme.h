
/*
(C) Kengo Kitamura
本プログラムを使用したことによるいかなる障害，損害も責任はとりません
各自の責任において使用してください
*/

/*
開発環境
NuGet
opencv3.1
opencv3.1 redist

・クラスの詳細
・Control.cpp
	アルゴリズムの流れを記述
	カメラの取得
・Ellipse.cpp
	Houghによる円近似
	目盛り領域を楕円で近似する
・Thinning.cpp
	針の芯線を出すため
・Labeling.cpp
	画像から目盛り領域を算出
・DgreeHistgram.cpp
	ヒストグラムを生成し目盛り領域の始終点を算出
	直線の検出
・Draw.cpp
	デバッグで画像を表示・保存
	アナログ値の算出
・maincode.cpp
	使用に用いるmain関数
*/
