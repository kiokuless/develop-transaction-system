= データ

データベースシステムの，データベースって何でしょうか．
ベースってのは基地という意味なので，データがたくさん整理された状態で管理されているくらいの意味だと思ってください．
じゃあデータって何でしょうか．この章ではデータとは何かについて考えます．



== 基本データ型とその等価性および順序

データのそれ以上分解できない基本的な単位を考えましょう．
それは，例えば，整数であったり，文字列であったり，日付だったり，バイナリデータだったりというものです．
それらを基本データ型(primitive data type)と呼びます．
基本データ型である整数型の具体的な値は例えば 1 や 2 です．これらの値は整数型のインスタンスともいいますね．
皆さんはデータベースシステムよりプログラミング言語の知識をたくさん持っていると思いますから，
プログラミング言語を語るときに用いられる用語を適宜持ってきて説明します．

同じ型の値をどうやって区別しましょうか．
それには等価性(equality)を使います．型 @<tt>{A} のデータ @<tt>{a1, a2} が等しいとき @<tt>{a1 == a2} とかき，
等しくないとき @<tt>{a1 != a2} と書きます．
基本データ型であれば値が同じであれば等しい，違えば等しくない，
という定義でまず実用上差し支えないでしょう．
たとえば，整数の 1 と 2 があったとき，@<tt>{1 == 1} で @<tt>{1 != 2} ですね．
文字列だと，@<tt>{'aaa' == 'aaa'，'aaa' != 'aab'，'aaa' != 'aaaa'} などです．

等価性に加えて，同じ型の値同士でよく用いられる関係が，順序(order) です．
特に全順序がよく使われます．
整数は全順序集合ですね．@<tt>{1 < 2} ですし，@<tt>{2 < 3} です．
日付も過去より現在，現在より未来が新しいという順序を持っています．
文字列は辞書順で大小関係を扱うことが多いです．
プログラミング言語においては，自分で作った構造体に任意の順序をつけることもありますね．
等価性は前提とすることが多いので @<tt>{==} が定義されており，さらに @<tt>{<} という演算子が定まれば
全順序を定義できます．


データベースを扱うときにデータの等価性はまず間違いなく必須ですが，
順序については必ずしも必要ありません．
データの部分集合を考えるとき，等価性のみ使えるデータ型はひとつずつ列挙する必要がありますが，
順序を持つデータ型は範囲で指定することができるという特徴があります．



== Record 型，Table，Key


データとは基本データ型の値の集合といえます．
RDBMS では基本データ型を複数まとめて，record 型というものを定義して使います．
Record 型の値は record と呼ばれ，tuple 型，tuple と呼ばれることもあります．
Tuple という言い方はプログラミング言語でもそのまま使いますね．
Record 型の中の要素を区別するために，それらを column とか field と呼び，
区別しやすいように名前をつけます．
Record 型は，プログラミング言語でいうところの構造体 (struct) に相当します．

RDBMS では record 集合を管理するために，table という概念が使われます．
ひとつの table には同じ record 型のデータが複数格納されます．
まったく同じ record 型の異なる table を作ることは可能です．
RDBMS では record 型には名前をつけ(られ)ず，table に名前をつけます．

RDBMS では record 型を入れ子にして定義する (nested) ことは想定されない場合が多いようです．
これはデータ重複を防ぐ「正規形」の考え方があるためだと思われます．
「正規形」「正規化」はデータベーススキーマ
(Record 型，table，key やその他の制約等の定義をまとめたもの)が持っているべき性質
やスキーマの正規形への変換方法を意味します．
トランザクション処理とは直接関係ないので，ここではこれ以上説明しません．
トランザクションでは，ひとつの record やその column を
それ以上分割されないデータアクセスの最小単位と考えます．

同じデータ型のふたつの record が等しいとは，素朴には全 column のデータが等しいことを意味します．
様々な場面で record の区別をするときに key という概念が使われます．
Key とは，record を入力とする関数もしくはその値と考えることができます．
より狭い定義では，record に含まれるひとつもしくは複数の column を指して key といいます．
以後，table の key と言ったときは，それに対応するひとつまたは複数 column を指すものとし，
ある record の key と言ったときは，具体的な record 内の対応する column 値(の tuple)を指すものとします．
Key が record を区別するのに十分な情報を持っているとき，すなわち
key が等価であることと record が等価であることが同値であるとき，その key は unique key といいます．
例えば，以下の例を考えます:

//list[record_key_example][]{
Table Human:
  id: integer
  last_name: string
  first_name: string
  birthday: date
  weight_g: integer
  height_mm: integer

Key Name of Human table:
  last_name
  first_name

Key NameAndBirthday of Human table:
  last_name
  first_name
  birthday

Unique key Id of Human table:
  id
//}

ここで @<tt>{Name} は unique key でしょうか？
その答えは扱う @<tt>{Human} table (型)が想定する record 集合に依存します．
つまり，データの管理者が unique key として扱いたい場合，それを満たすようシステムに制約を与えます．
具体的には @<tt>{Name} を unique key にすると管理者が決めたならば，
システムは @<tt>{(last_name, first_name)} の組が unique でなくなるような操作を許しません．
たとえば，既に @<tt>{('tanaka', 'ichiro')} という Name key を持つ record が存在するのに，同じ
@<tt>{('tanaka', 'ichiro')} を持つ別の record は，例え他の column が異なっていても追加できなくなります．
(逆に，典型的な RDBMS は unique 制約がなければ何もかも同じ record を複数登録できます．)
もちろん一般には @<tt>{Name} は unique ではないので，Name を unique key にすると不便です．
ならば @<tt>{NameAndBirthday} はどうでしょうか．
実際に同性同名で誕生日も同じの人がいる可能性はかなり低いでしょうがゼロというわけでもないでしょう．
一般的なデータベース設計では，意図的に uniqueness を担保するために，無意味な unique id を割り当てて
unqiue key として区別できるようにします．
今ならマイナンバーがあり，個人にひとつ割り当てられることを仕組みとして担保しているので，
システム毎に @<tt>{id} を割り振るよりも，unique key としてマイナンバーを使った方が良いかも知れませんね．
個人情報の保護など別の社会的法律的な制約が発生しますけれど:)

ある table があったとき，その unique key の中で主要なものをひとつ選び，primary key と呼びます．
明示的な primary key がない場合は，隠し column が用意され，
table 内で unique な整数が割り当てられ，record identifier として扱われることが多いです．
(MySQL InnoDB はそのような実装となっています．)


複数の columns から構成される Key の順序を考える場合は辞書順を考えます．
2 つの整数型からなる key があって，@<tt>{(a, b)} と表すとき，
その key の順序は例えば @<tt>{(1, 1) < (1, 2) < (2, 1)} となります．
もちろん，任意の key について，任意の順序を定義し得るわけですが，
自動的に決まる順序として辞書順が採用されるシステムが多いです．
逆に，ある順序で扱いたいからそうなるように key を定義するとも言えるでしょう．
RDBMS の実装によっては，基本データ型が持つ自然な順序を ascending (昇順)，
その逆順を descending (降順) としてそれぞれ @<tt>{ASC, DESC} の演算子で扱えるものがあります．

ある table について，任意の key を指定すれば，複数の record がマッチし得ます(もちろん 0 個のこともあります)．
Unique key の場合は高々 1 つの record がマッチします．
データベースにおいてデータを指定する最も基本的な操作が，ある table において
key 値を与えて table を構成する record 集合の部分集合を指定する操作です．
等価性を用いる場合は，指定したい等価な key 値の集合を与えます．
順序を用いる場合は，指定したい key 値の範囲を与えます．
演算子や関数を使ってより複雑な条件を指定することも出来ますが，
最終的には，複数 table の複数 record に何らかの順番でアクセスすることになります．

====[column] 順序と全順序

集合が順序や全順序を持つためにはある性質を満たす必要があります．
具体的には集合の元についての二項関係を表す演算子 @<tt>{<=} が反射律，反対称律，推移律を満たせば(半)順序であり，
加えて，集合の任意の 2 つの元が @<tt>{<=} で比較可能である場合に全順序といいます．

単純なルールとして，その型の任意の値を整数や実数に割り当てる関数(写像)を用意すれば，全順序の性質を満たします．
整数や実数の tuple に割り当てても良いです．全順序の型で作る tuple 型は辞書順を考えれば全順序となります．
Key は基本データ型の tuple でしたね．Key を構成する全ての column の型が全順序の性質を持っていれば，
Key にも自然な全順序が定義されます．

====[/column]




== データの関係とポインタ


データは構造(関係)を持っています．
プログラミング言語では，基本データ型に加えて，
構造体とポインタ(参照)型があれば，
任意のデータ構造を表現することが出来るでしょう@<fn>{footnote_array_type}．

//footnote[footnote_array_type][配列型？私は好きですよ．主にヒープメモリに確保する動的配列が大好きです．]

Relational database systems (関係データベースシステム，以後 RDBMS とかく)は，
ポインタ型を直接的には扱わない特徴があります．
では RDBMS でデータ同士の関係を表すにはどうすれば良いのでしょうか．
それは，共通の部分データを持つことで表現します．
R1, R2 という record 型があり，それぞれが C1 という column を持つものとします．
R1.C1 (R1 型における C1 column という意味) と R2.C1 が等価である
レコード同士，すなわち，@<tt>{R1.C1 == R2.C1} である R1 型の record と R2 型の record は
関係があるという意味になります．
同一 record 型 (もしくは 同一 table 内)の record 同士に関連を持たせたい場合は，
R1.C1 と R1.C2 という二つの column を定義しておいて，@<tt>{R1.C1 == R1.C2} という形で関係を持たせることができます．
これらの関係は，一般に，1:1，1:N，M:N という 3 種類のパターンに分類して考えます．

無理矢理ポインタでどのような表現になるかを考えてみましょう．
1:1 の関係は，record と record をお互いがお互いを指している状態を表します．
1:N は，N 側の各 record が 1 側の record を指している状態，1 側は配列などを持っていて，そこに N 側を指すポインタが
複数格納されている状態を表します．
M:N は M 側の各 record が配列などで N 側の record を指すポインタを複数保持している状態と，
N 側の各 record が配列などで M 側の record を指すポインタを複数保持している状態と考えることができます．
こんな複雑な関係をポインタで管理したくないですね:)


何故ポインタを使わないか，という問いには歴史的経緯があるようなので，
興味のある人は調べてみてください@<fn>{footnote_no_pointer}．
ポインタを扱わないことで多少窮屈ですが dangling pointer がない世界に住むことができます．

//footnote[footnote_no_pointer][私も詳しくないですが，http://leoclock.blogspot.com/2009/01/blog-post_07.html によると，Readings in Database Systems の中の解説記事に，書いてあるようです．]




== 最も単純なデータベースのスキーマ

最も単純なデータベースについて考えてみましょう．
まず table がひとつしかないです．
その table 型が採用する record 型は key を表す型と value を表す型の 2 つの column で表現される
record 型です．Key は primary key すなわち unique key です．
これは key-value store と呼ばれるものですね．
Key は文字列型で，value はバイト列であることが多いです．
Key は文字列型として自然に定義される等価性と順序(辞書順)をサポートしています．

アプリケーションが数値型を必要とするなら，数値型を文字列型に変換して使います．
10進数を用いて数字を文字として足りない分を 0 埋めして桁数を固定した文字列に変換すれば，
辞書順と数値の昇順は一致させることができます．
もちろん文字集合における順序が @<tt>{'0' < '1' < ... < '9'} を満たすことが前提となります．
例えば 10 桁固定にするとして 1 は '0000000001' に変換され，10 は '0000000010' に変換されます．
'0000000001' < '0000000010' ですね．
負の数を扱いたいならコンピュータがそうしているように，補数を文字列に変換する必要があります．

スキーマを自在に定義できるデータベースシステムはもちろん実用では必須ですが，
学習用として最初に作るべきは，このような単純な key-value を管理するシステムでしょう．



====[column] コンピュータサイエンスで使う数学

あなたがデータ構造やアルゴリズムについて考察したり，生み出したりしたとき，
定式化をする必要が出てくることがあります．
コンピュータサイエンスで用いる数学について学ぶのに良い資料として，

 * Mathematics for Computer Science.
 ** Eric Lehman, F Thomson Leighton, and Albert R Meyer.
 ** https://courses.csail.mit.edu/6.042/spring17/mcs.pdf

を挙げておきます．
特に，集合の基礎と述語論理のところをきちんと学んで使えるように練習することが，
論文を読むときの定理や証明を理解するための第一歩であり，自分で定式化するときの道具にもなると思います．
まあ私はごく最近この資料の存在を知って勉強したので偉そうなことは言えません．
この資料のライセンスは CC BY-SA 3.0 だそうです．太っ腹ですね．

====[/column]




====[column] Null について


QQQQQ


====[/column]






====[column] メモ

* 後で本文を分かりやすく修正する．

・「record型のkeyは複数columnのこと」とのことですが、うまく意味が理解できないです。recordのkeyというのは、あるcolumnに属するrecordに入っている値のことというのはわかったのですが。SQL文で言うWHERE id = '〇〇'のidがrecord型のkeyで〇〇がrecordのkeyというように、抽象と具体の対比みたいなことでしょうか？
・「Keyの全順序を…」のところなのですが、いまいちどういうことなのかわからないです。keyを複数個指定して、それに該当したrecordはそれぞれの要素に対して辞書順で並ぶってことさえわかればいいってことですか？2つのintで構成されるkeyというのは、データが構造体で入っているということなのか、それともintのkeyが2つあるということなのですか？


> ・「record型のkeyは複数columnのこと」とのことですが、うまく意味が理解できないです。recordのkeyというのは、あるcolumnに属するrecordに入っている値のことというのはわかったのですが。SQL文で言うWHERE id = '〇〇'のidがrecord型のkeyで〇〇がrecordのkeyというように、抽象と具体の対比みたいなことでしょうか？

そうですそうです。分かりにくくてすいません。
SQL の例では、id が key (のひとつ)で、○○は key data ですね。

例えば、ある record 型が、(name, age, score) の 3 columns で定義されているとき、name, age, score の任意の組み合わせ、すなわち
(name), (age), (score), (name, age), (age, score), (name, score), (name, age, score), ...(順番を考慮すればもっとたくさん)
の key の候補があります。(我々は、ある table を考えるとき、それらのたくさんある key 候補のうち、一部のみを実際に key として扱います。)
key という概念は、record を絞り込むためのもので、
r1 (alice, 18, 80)
r2 (bob, 20, 80)
r3 (cris, 20, 80)
r4 (dany, 20, 90)
という table (record 集合) があったときに、
(age, score) = (20, 80) を与えると r2 と r3 の record がマッチします(絞り込まれます)。
「具体的な record の key が。。。」という表現は、例えば (cris, 20, 80) という record の (age, score) key の値は (20, 80) ですよ、という意味です。。


> ・「Keyの全順序を…」のところなのですが、いまいちどういうことなのかわからないです。keyを複数個指定して、それに該当したrecordはそれぞれの要素に対して辞書順で並ぶってことさえわかればいいってことですか？2つのintで構成されるkeyというのは、データが構造体で入っているということなのか、それともintのkeyが2つあるということなのですか？

「Key の全順序」というのは、言葉が足りなかったかと思います。
「複数 column から成る Key の全順序」という意味です。
ひとつの column から成る key の全順序が決まっていたときに、複数 column から成る key の順序が辞書順として自然に決まる
ことが多いし、一般の RDBMS はそれを仮定するという意味です。。元々は、好きに順序は定義しても良いはずですが、

2 つの int で構成される key (column1, column2) の data は、(a, b) という形の data です。
 「データが構造体で入っている」ということです。(階層構造はあまり考えないので tuple 型と言った方が良いかも)

絞り込む、という点からすれば、
Key1: (age)
Key2: (score)
Key3: (age, score)
の 3 つの key があったとして、
(age, score) = (20, 80) の key data を与えて絞り込む (where age = 20 and score = 80 に相当) には、
(1) Key1 で絞り込む --> [r2, r3, r4]  --> Key2 で絞り込む --> [r2, r3]
(2) Key2 で絞り込む --> [r1, r2, r3]  --> Key1 で絞り込む --> [r2, r3]
(3) Key3 で絞り込む --> [r2, r3]
という 3 通りの絞り込み方があります。
なんでこんなややこしいことを考えるかというと、絞り込みを効率化するデータ構造として index というものがあり、
それをうまく使う余地があるからです。


====[/column]


