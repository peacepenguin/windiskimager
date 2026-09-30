<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP" sourcelanguage="en_US">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>カスタムパーティション</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>イメージファイルまたはディスクを追加し、デバイスまたは新しいイメージファイルに配置するパーティションにチェックを付けて、並べ替えてください。各ソースのパーティションテーブルは先頭のセクタから読み取られます。書き込むか完全スキャンを指示するまで、それ以外は読み取りません。</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>ソース</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>ソース / パーティション</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>種類</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>サイズ</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>イメージを追加...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>ディスクを追加...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>ディスクからもパーティションを取り込みます: カード、USB ドライブ、その他のディスク。Windows が起動しているディスクは表示されません。ディスクの読み取り中は、そのボリュームはロックされ、マウント解除されます。</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>完全スキャン</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>イメージ全体を読み取って展開し、正確なサイズを調べ、すべてのパーティションが末尾まで含まれているかを確認します。パーティションテーブルがなく、ファイルにサイズが記録されていないイメージの場合、または書き込み前に圧縮イメージを確認する場合にのみ必要です。</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>レイアウト</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>パーティション (順番):</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>上へ</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>下へ</translation>
    </message>
    <message>
        <source>Free space:</source>
        <translation>空き領域:</translation>
    </message>
    <message>
        <source> MiB</source>
        <translation> MiB</translation>
    </message>
    <message>
        <source>How much unpartitioned space to insert, or, with free space selected in the order, how much it is. The partition after it still starts on a 1 MiB boundary.</source>
        <translation>挿入する未割り当て領域のサイズ、または順番で空き領域を選択している場合はそのサイズです。その後のパーティションは、それでも 1 MiB 境界から始まります。</translation>
    </message>
    <message>
        <source>Insert</source>
        <translation>挿入</translation>
    </message>
    <message>
        <source>Leave this much unpartitioned space after the selected item of the order, or at the end.</source>
        <translation>順番で選択した項目の後、または末尾に、このサイズの未割り当て領域を残します。</translation>
    </message>
    <message>
        <source>Remove free space</source>
        <translation>空き領域を削除</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>先頭領域の取得元:</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>このイメージのブートコードと、パーティションテーブルから最初のパーティションまでの領域 (最大 32 MiB) をコピーします。この領域にはブートローダーが格納されている場合があります。デバイスにはこのイメージと同じ種類のパーティションテーブルが作られ、最初のパーティションはこのイメージと同じ位置から始まります。</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>デバイス上</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <source>From</source>
        <translation>取得元</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>書き込み先</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>デバイス:</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>すべてのデバイスを表示</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>固定ディスクも一覧に表示します。内蔵 PCIe カードリーダーはカードをリムーバブルでないデバイスとして見せることが多く、その場合は通常表示されません。Windows が起動しているディスクが一覧に出ることはありません。</translation>
    </message>
    <message>
        <source>Keep the device&apos;s partitions</source>
        <translation>デバイスのパーティションを保持</translation>
    </message>
    <message>
        <source>Add to what the device holds instead of replacing it: its partitions stay where they are, untouched, and the new ones go into its free space, where the order puts them. Untick one of them in the order to take it out of the table; its space is then free. The table keeps its kind, and its backup GPT is moved to the end of the device.</source>
        <translation>デバイスの内容を置き換えずに追加します: 既存のパーティションはそのままの位置に変更されずに残り、新しいパーティションは順番で指定された位置の空き領域に配置されます。順番で既存のパーティションのチェックを外すと、テーブルから外され、その領域は空きになります。テーブルの種類は変わらず、バックアップ GPT はデバイスの末尾に移動されます。</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>イメージファイル:</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation type="vanished">combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>参照...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>圧縮形式</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>書き込み時の圧縮形式: .img.zst が最も速く、.img.xz が最も小さく、.img.gz が最も広く対応しています</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>書き込み後に照合</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>書込み...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>書き込み先のデバイスが選択されていません</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>ディスク %1 を読み取れませんでした</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>ディスク %1 のセクタは %2 バイトですが、ソースのセクタは %3 バイトです</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>ディスク %1: %2</translation>
    </message>
    <message>
        <source>disk %1 could not be read: %2</source>
        <translation>ディスク %1 を読み取れませんでした: %2</translation>
    </message>
    <message>
        <source>disk %1 has no partition table to keep</source>
        <translation>ディスク %1 には保持するパーティションテーブルがありません</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>結合したイメージの保存先</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>ディスクイメージ (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
    </message>
    <message>
        <source>%1 GB</source>
        <translation>%1 GB</translation>
    </message>
    <message>
        <source>%1 MB</source>
        <translation>%1 MB</translation>
    </message>
    <message>
        <source>%1 KB</source>
        <translation>%1 KB</translation>
    </message>
    <message>
        <source>the image ends inside its own partition table</source>
        <translation>イメージが自身のパーティションテーブルの途中で終わっています</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>パーティションテーブルを読み取れませんでした</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>イメージを追加</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 は使用できません: %2。</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 にはパーティションテーブルがないため、イメージ全体を 1 つのパーティションとして扱います。ファイルにはそのサイズが記録されていないため、末尾まで読み取って調べる必要があります。

今すぐスキャンしますか？</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>ディスクを追加</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>パーティションを取り込むディスクにチェックを付けてください:</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>固定ディスクも一覧に表示します。Windows が起動しているディスクが一覧に出ることはありません。</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- 書き込み先のデバイス</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>すでにソースに含まれています。</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>ディスク %1 は使用できません: %2。</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>ディスク</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>%1 をスキャンしています...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>%1 を末尾まで読み取れませんでした: %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>%1 をスキャンしています: %2 読み取り済み...</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 はパーティション %3 の終わりより前の %2 で終わっています。イメージは不完全で、そのパーティションを丸ごとコピーすることはできません。</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>イメージ全体</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>パーティション %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>パーティション %1: %2</translation>
    </message>
    <message>
        <source>%1, %2</source>
        <translation>%1、%2</translation>
    </message>
    <message>
        <source>GPT</source>
        <translation>GPT</translation>
    </message>
    <message>
        <source>MBR</source>
        <translation>MBR</translation>
    </message>
    <message>
        <source>no partition table</source>
        <translation>パーティションテーブルなし</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>サイズ未記録</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1、スキャン済み</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>不明: イメージをスキャンしてください</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>無し: 新しい空のテーブル</translation>
    </message>
    <message>
        <source>Free space: %1 MiB</source>
        <translation>空き領域: %1 MiB</translation>
    </message>
    <message>
        <source>This device, %1</source>
        <translation>このデバイス、%1</translation>
    </message>
    <message>
        <source>%1 -- taken out of the table</source>
        <translation>%1 -- テーブルから除外</translation>
    </message>
    <message>
        <source>%1 -- kept</source>
        <translation>%1 -- 保持</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>デバイスに配置するパーティションにチェックを付けてください。</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>書き込めません: %1。</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>書き込めません: %1 は書き込み先のデバイスです。イメージファイルに書き込むか、別のデバイスを選択してください。</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>パーティションテーブル (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>先頭領域</translation>
    </message>
    <message>
        <source>Free space</source>
        <translation>空き領域</translation>
    </message>
    <message>
        <source>%1, kept</source>
        <translation>%1、保持</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>バックアップ GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1、%2 個のパーティション: %3 のイメージファイル。</translation>
    </message>
    <message>
        <source>%1, %2 partitions, %3 of them kept: %4 used, %5 free of %6.</source>
        <translation>%1、%2 個のパーティション (うち %3 個を保持): %6 のうち %4 使用、%5 空き。</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1、%2 個のパーティション: %5 のうち %3 使用、%4 空き。</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>サイズが記録されていないイメージは、スキャンまたは書き込みの時にのみ確認されます。</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>GUID が重複しているパーティションがあります。これについては後で確認されます。</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>書き込むイメージファイルの名前を指定してください。</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 は結合するイメージの 1 つです。別の名前を選択してください。</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 は既に存在します。上書きしますか？</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 はソースの 1 つであるディスク %2 上にあります。読み取り中はそのボリュームがロックされるため、書き込むことができません。別のディスク上の場所を選択してください。</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>パーティション GUID の重複</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>以下の一意パーティション GUID が、選択した複数のパーティションに属しています:

%1

通常、これは 1 つのイメージの 2 つのコピーから同じパーティションを取り込んだものです。GUID が重複していると、fstab やカーネルコマンドラインで PARTUUID によってパーティションを探すシステムが、誤ったほうを使う可能性があります。

後のコピーには新しい GUID を生成できます。最初のものは元の GUID を保持します。その場合、再生成したパーティションを古い PARTUUID で指定しているものは、そのパーティションを見つけられなくなります。</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>新しい GUID を生成</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>そのままにする</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation type="vanished">Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>イメージファイル</translation>
    </message>
    <message>
        <source>...</source>
        <translation>…</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>照合</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">読み取り時にイメージを縮小</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">デバイスの MBR または GPT を読み取り、実際のパーティションに合わせてイメージを縮小します。バックアップ GPT は使用済み領域の末尾へ移動します。</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">.img.gz として読み取る</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">デバイスから読み取ったイメージを gz で圧縮します</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">.img.xz として読み取る</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">デバイスから読み取ったイメージを xz で圧縮します</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">読み取るパーティションを選択</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">読み取りの前に、デバイスのパーティションを一覧表示し、含めるものを選択できます。除外したものはイメージから取り除かれます。これは未パーティション領域と同様で、「読み取り時にイメージを縮小」がチェックされているかどうかにかかわらず、常にイメージを縮小します。</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>WinDiskImagerを終了</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Win Disk Imagerを終了</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">GPT を確認</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>選択中のデバイスの GPT が壊れていないかを確認し、壊れていれば修復を提案します。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">未パーティション領域をスキップ</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">デバイスのMBRまたはGPTを読み取り、未パーティション領域をイメージから除きます。パーティション、パーティションテーブル、およびGPTがパーティションの前に予約している領域は残します。バックアップGPTはイメージの新しい末尾に移動します。</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>読み取り時に圧縮</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>デバイスから読み取ったイメージを、下で選択した形式で圧縮します</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">読み取り時の圧縮形式: .img.gz は作成が速く、.img.xz はサイズが小さくなります</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">デバイスのMBRまたはGPTを読み取り、パーティション間とその後ろにある未パーティション領域を除きます。ブートローダーが置かれる最初のパーティションより前の領域は、パーティションテーブルの後ろ32 MBまではそのまま読み取り、それを超える部分だけを除きます。バックアップGPTはイメージの新しい末尾に移動します。</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>デバイスのMBRまたはGPTを読み取り、パーティション間とその後ろにある未パーティション領域を除きます。最初のパーティションより前の領域のうち、ブートローダーが未使用領域に置かれている可能性のある最大32 MBはそのまま読み取り、それを超える部分だけを除きます。バックアップGPTはイメージの新しい末尾に移動します。</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">読み取りの前に、デバイスのパーティションを一覧表示し、含めるものを選択できます。除外したものは未パーティション領域と同様にイメージから取り除かれます。この場合、「未パーティション領域をスキップ」がチェックされているかどうかにかかわらず、未パーティション領域も常にスキップされます。</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>読み取るパーティションを選択</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>デバイスのパーティションの一部だけを読み取ります: デバイスをソース、すべてのパーティションにチェックを付けた状態、イメージファイルを出力先として「カスタムパーティション」を開きます。除外するもののチェックを外してください。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>読み取り時に未パーティション領域をスキップ</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>ツール</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>デバイスの GPT を確認</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>カスタムパーティション...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>イメージファイルやディスクのパーティションを、選択した順序で、新しいパーティションテーブルのもとにデバイスまたは新しいイメージファイルへ配置します。</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>イメージファイルのハッシュ</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>イメージファイルに対して生成するハッシュの種類</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無し</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>選択したハッシュをファイルから生成</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>作成</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>ハッシュをクリップボードにコピー</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>コピー</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>書き込み後に GPT を修正する</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>書き込み後、バックアップ GPT をデバイスの末尾へ移動し、ヘッダーを一致するよう更新して、Windows が「修復」するものを残しません。チェックを外すと、代わりにデバイスを取り外すよう警告します。</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>すべてのデバイスを表示</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>固定ディスクも一覧に表示します。内蔵 PCIe カードリーダーはカードをリムーバブルでないデバイスとして見せることが多く、その場合は通常表示されません。Windows が起動しているディスクが一覧に出ることはありません。</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">デバイスのMBRまたはGPTを読み取り、パーティション間とその後ろにある未パーティション領域を除きます。ブートローダーが置かれる最初のパーティションより前の部分はそのまま読み取り、最初のパーティションは移動しません。バックアップGPTはイメージの新しい末尾に移動します。</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>読み取り時の圧縮形式: .img.zst が最も速く、.img.xz が最も小さく、.img.gz が最も広く対応しています</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>進捗状況</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>現在の処理をキャンセルします。</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>デバイスからファイルを作成</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>読込み</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>ファイルからデバイスに書込み</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>書込み</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>デバイスとファイルを照合</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">イメージとドライブの照合</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">照合のみ</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Win32 Disk Imagerを終了</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>終了</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>終了しますか？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したイメージファイルが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>終了すると、破損したディスクが作成されます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>イメージを選択</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>生成中…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>キャンセルしますか？</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>いまキャンセルすると、宛先が破損します。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>イメージファイルをデバイスに配置できません。</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">タスクを待っています。</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>終了すると、照合がキャンセルされます。
本当に終了してもよろしいですか？</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>照合をキャンセルします。
本当にキャンセルしてもよろしいですか？</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>空き容量が足りません！</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">書き込みは成功しましたが、パーティションテーブルが危険な状態です。

%1
%2

%3

他の操作を行う前に、今すぐデバイスを物理的に取り外してください。このコンピューターに再度挿入せず、目的のハードウェアに挿入してください。</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>選択したファイルは存在しません。</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>指定されたファイルにはデータが含まれていません。</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>完了しました。</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>完了</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>書き込み成功。</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>ディスクイメージ (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>圧縮ディスクイメージ (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>エラー</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">チェックサムを生成するためにファイルを開けませんでした:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>書き込み先のデバイスを選択してください。</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>このデバイス上のすべてのファイルとデータが削除されます。
(書き込み先デバイス: %1)
本当に続行してもよろしいですか？</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>デバイスにマウント済みのボリュームがあります</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 は Windows で %2 としてマウントされています。

このデバイス上のすべてのパーティションのデータはすべて破棄され、復元できません。

%2 が残しておきたいドライブでないことを確認してください。

それでもこのデバイスに書き込みますか？</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>書き込みに失敗しました。</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>デバイスがサイズ 0 を報告しています。カードリーダーの場合、カードが取り外された可能性があります。</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>ハッシュを計算するためにファイルを開けませんでした:
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>ハッシュ計算中…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>ハッシュを計算するためにファイル全体を読み取れませんでした:
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>ハッシュの計算をキャンセルしました。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>イメージがデバイスより大きいです:
  イメージ: 少なくとも %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

イメージの末尾は書き込まれないため、デバイスには完全なイメージが入りません。

それでも続行しますか？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

イメージが圧縮されているため、超過部分にデータがあるか確認できませんでした

それでも続行しますか？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

超過部分にはデータが含まれているようです

それでも続行しますか？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>使用可能な容量を超える容量が必要です:
  必要: %1 セクタ
  使用可能: %2 セクタ
  セクタサイズ: %3

超過部分にデータは含まれていないようです

それでも続行しますか？</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>書き込みを中止しました。</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>古いパーティションテーブルを消去しています…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>デバイス上の既存のパーティションテーブルを消去できませんでした。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>デバイスへの書き込みが途中で終わったため、使用できるイメージが含まれていません。使用する前にイメージを書き込み直してください。</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>GPT を修正しています…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>イメージが途中で切れています</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">書き込みに成功しました。

GPT をデバイス (%1) と整合するようにしたため、Windows が修復すべき壊れたテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">書き込みに成功しました。

イメージに GPT が含まれていないため、Windows が修復するパーティションテーブルはありません。デバイスは通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>書き込みに成功しました。</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>書き込み成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">デバイスをオフラインにして取り出しました。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">デバイスを自動的にオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">GPT を自動的に修正できませんでした (%1)。</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT の形式が不正です</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>GPT の修正に失敗しました (%1)。</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">「書き込み後に GPT を修正する」オプションが有効になっていません。</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">このイメージは Windows の GPT 書き換え不具合の影響を受けます。

最初のパーティションの前に領域を確保しているため、再スキャン時に Windows がプライマリパーティションテーブルを書き換え、誤ったセクタを指すようになります。その結果は Windows 自身の検査は通りますが、Linux では拒否され、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">このイメージは Windows の GPT 書き換え不具合の影響を受けません。

バックアップ GPT がデバイスの末尾にないため、再スキャン時に Windows はテーブルを書き換えますが、この配置では書き換え後も正しい値になります。いずれにせよ、今デバイスを取り外せばイメージとバイト単位で同一のまま保てます。</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">このイメージが Windows の GPT 書き換え不具合の影響を受けるかどうかを判定できませんでした。影響を受けるものとして扱ってください。再スキャンにより、Linux が拒否するパーティションテーブルが残り、デバイスが起動しなくなる可能性があります。</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>今すぐデバイスを取り外してください</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>選択したファイルを読み取る権限がありません。</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">イメージは非圧縮でのみ読み出せます。.gz や .xz の拡張子を付けないファイル名を選んでください。

圧縮イメージ (.img.gz、.img.xz) は書き込みと検証には使用できます。</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>読み込みに失敗しました。</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>検証に失敗しました。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>イメージがデバイスより大きいです:
  イメージ: 少なくとも %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

収まる部分のみ比較できます。

それでも続行しますか？</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

イメージが圧縮されているため、超過部分にデータがあるか確認できませんでした

それでも続行しますか？</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>検証を中止しました。</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>検証中…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>パーティションテーブルの破損</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>修復に失敗しました</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>パーティションテーブルを修復できませんでした: %1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">イメージに含めるパーティションを選択してください。</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>セクタ %1 でデバイスを読み取れませんでした。</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>デバイスにはイメージが正しく書き込まれていますが、パーティションテーブルが壊れています:</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>イメージがデバイスより大きいです</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>デバイスにはイメージが正しく書き込まれていますが、パーティションテーブルはまだ壊れています。「書き込み後に GPT を修正する」をオンにしてイメージを書き込み直すか、もう一度検証を実行して修復を実行してください。</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>検証に成功しました。

デバイスのパーティションテーブルは破損していましたが、修復しました。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>検証に成功しました。

イメージとデバイスの違いは GPT のみで、デバイス上の GPT は正常です。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>検証に成功しました。

イメージとデバイスの違いは GPT のみです。</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[ディスク %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>使用するイメージファイルを指定してください。</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>ディスクを検索しています…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">チェックサムを生成するためにファイル全体を読み取れませんでした:
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>書き込み中：%1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>読み込み中：%1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>検証中：%1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>ハッシュ計算中：%1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">チェックサムを生成中…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">チェックサムの生成をキャンセルしました。</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1。プライマリ GPT ヘッダーが、パーティションエントリの存在しないセクタを指しています。

これは、「書き込み後に GPT を修正する」を使わずに書き込んだカードを Windows が再スキャンしたときに残る状態です。データは失われていませんが、デバイスは起動せず、多くのツールがこのテーブルを受け付けません。

今すぐパーティションテーブルを修復しますか？</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>デバイスを選択してください。</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>デバイスをロックできませんでした。</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>デバイスを開けませんでした。</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>このデバイスのパーティションテーブルは壊れています:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>パーティションテーブルを修復しました。</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>パーティションテーブルはまだ破損しています。</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>パーティションテーブルは正常です。</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>パーティションテーブル</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>このデバイスの GPT は正常です。ヘッダーと、それが指すパーティションエントリは一致しています。</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>このデバイスに GPT はありません。</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>このデバイスには GPT がないため、ここで確認する破損は発生しません。</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>パーティションテーブルを読み取れませんでした。</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>パーティションテーブルを読み取れなかったか、この機能が修復できる破損とは別の形で壊れています。</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>書き込み先のデバイスがソースの 1 つにもなっています。</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 は書き込み先のデバイス上にあるため、そのデバイスに書き込むことはできません。</translation>
    </message>
    <message>
        <source>Confirm write</source>
        <translation>書き込みの確認</translation>
    </message>
    <message>
        <source>The device keeps the partitions ticked in the order, and they are not written to. Anything in its free space, and in the partitions taken out of its table, may be overwritten.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>デバイスは順番でチェックを付けたパーティションを保持し、それらには書き込みません。空き領域と、テーブルから外したパーティションにあるものは上書きされる可能性があります。
(書き込み先デバイス: %1)
本当に続行してもよろしいですか？</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Its volumes are dismounted while it is written, and the device is ejected afterwards. The kept partitions are not changed.

Write to this device anyway?</source>
        <translation>%1 は Windows で %2 としてマウントされています。

書き込み中はボリュームがマウント解除され、書き込み後にデバイスは取り出されます。保持するパーティションは変更されません。

それでもこのデバイスに書き込みますか？</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>確認中にデバイスの一覧が変わりました。書き込み先のデバイスを確認して、もう一度お試しください。</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 は、そこに提供するパーティションの終わりより前のセクタ %2 で終わっています。イメージは不完全です。</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>デバイスのセクタ %1 が %3 のセクタ %2 と一致しません。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>デバイスは途中まで書き込まれており、使用可能なレイアウトではなくなっています。使用する前にもう一度書き込んでください。</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has changed since Custom Partitioning read it. Open Custom Partitioning again to plan from what the device holds now.</source>
        <translation>「カスタムパーティション」が読み取った後に、デバイスのパーティションテーブルが変更されました。現在のデバイスの内容に基づいて計画するには、「カスタムパーティション」を開き直してください。</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>書き込み中…</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has not been changed: it holds its partitions as before. Its free space may hold part of the new ones.</source>
        <translation>デバイスのパーティションテーブルは変更されていません: パーティションは以前のままです。空き領域には新しいパーティションの一部が含まれている可能性があります。</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>デバイス上のパーティションテーブルが書き込んだ内容と一致しません。</translation>
    </message>
    <message>
        <source>GPT</source>
        <translation>GPT</translation>
    </message>
    <message>
        <source>MBR</source>
        <translation>MBR</translation>
    </message>
    <message>
        <source>The device&apos;s %1 partition table now holds %2 partitions: %3 kept, and %4 new from %5 images.</source>
        <translation>デバイスの %1 パーティションテーブルには現在 %2 個のパーティションがあります: 保持 %3 個、%5 個のイメージからの新規 %4 個。</translation>
    </message>
    <message>
        <source>Write and verify successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>書き込みと照合に成功しました。

デバイスには、%3 個のイメージから取り込んだ %2 個のパーティションを持つ新しい %1 パーティションテーブルがあります。</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>書き込みに成功しました。

デバイスには、%3 個のイメージから取り込んだ %2 個のパーティションを持つ新しい %1 パーティションテーブルがあります。</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>そのバックアップは既にデバイスの末尾にあるため、Windows が修復するものはありません。</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>起動するかどうかは、ブートローダーがパーティションを現在の位置で見つけられるかどうかによります。</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>カスタムパーティション</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、末尾は書き込まれず、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスがいっぱいになるまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>書き込みに成功しました。

GPT がデバイス (%1) と一致したため、Windows が修復するものはありません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>書き込みに成功しました。

このイメージは GPT ではなく MBR パーティションテーブルを使用しているため、Windows の GPT 書き換えの不具合の影響を受けません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>書き込みに成功しました。

このイメージにはパーティションテーブルがないため、Windows の GPT 書き換えの不具合の影響を受けません。通常どおり取り外せます。</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>デバイスはオフラインにされ、取り出されました。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>デバイスをオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>GPT を修正できませんでした (%1)。</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>不正な GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>「書き込み後に GPT を修正する」はオフです。</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>このイメージは影響を受けます。最初のパーティションの前に領域を確保しているため、再スキャンによりプライマリテーブルが誤ったセクタを指します。Windows は結果を受け入れますが Linux は受け入れず、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>このイメージは影響を受けません。再スキャンではテーブルが書き換えられますが、このレイアウトでは正しい値が書き込まれます。今取り外せば、いずれにせよイメージと同一のままです。</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>このイメージが影響を受けるかどうかは判別できませんでした。影響を受けるものとして扱ってください。再スキャンにより Linux が拒否するテーブルが残り、デバイスは起動しません。</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>書き込みに成功しましたが、パーティションテーブルが危険な状態です。

%1 %2

%3

今すぐデバイスを取り外し、このパソコンに再挿入しないでください。そのまま対象のハードウェアに差し込んでください。</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>結合したイメージが途中で終わりました。</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>%2 のセクタ %1 が書き込んだ内容と一致しません。</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 に結合したイメージより多くのデータが含まれているか、正しく終わっていません。</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>書き込みと照合に成功しました。</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 には、%4 個のイメージから取り込んだ %3 個のパーティションを持つ %2 パーティションテーブルがあります。</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>バックアップ GPT はイメージの末尾にあります。「書き込み後に GPT を修正する」を使うと、イメージを書き込むときに、より大きいデバイスの末尾へ移動されます。</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">パーティションの選択</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">未パーティション領域をスキップすると、パーティションとパーティションテーブル、およびGPTがパーティションの前に予約している領域だけが残ります。

シングルボードコンピューター用など一部の起動可能なイメージは、ブートローダーのデータをパーティションの外に置いています。そのようなデバイスをこの方法で読み取ったイメージは起動しない場合があります。</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">縮小すると、パーティションとパーティションテーブル、およびGPTがパーティションの前に予約している領域だけが残ります。

シングルボードコンピューター用など一部の起動可能なイメージは、ブートローダーのデータをパーティションの外に置いています。そのようなデバイスの縮小イメージは起動しない場合があります。</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">イメージに含めるパーティションを選択してください。チェックを外したものは、未パーティション領域と同様に取り除かれます。</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">パーティション %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">パーティション %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">少なくとも1つのパーティションはチェックしたままにしてください。</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>読み込みエラー</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">イメージは非圧縮でのみ読み戻せます。.gz または .xz 拡張子を使わないファイル名を選択するか、「.img.gz として読み取る」または「.img.xz として読み取る」をオンにしてください。</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>読み込み元のデバイスを選択してください。</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>上書きの確認</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>ファイルを上書きしてもよろしいですか？</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">デバイスにパーティションテーブルが見つからなかったため、選択できるものがありません。デバイス全体を読み取ります。</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">読み取りをキャンセルしました。</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>指定されたイメージに対しディスク容量が十分ではありません。</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>読み込み中…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>読み込みがキャンセルされました。</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>読み込み成功。</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>ファイル情報</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>データを保存するファイルを指定してください。</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>照合エラー</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>照合するデバイスを選択してください。</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

超過部分にはデータが含まれているようです

それでも続行しますか？</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>イメージのサイズがデバイスより大きいです:
  イメージ: %1 セクタ
  デバイス: %2 セクタ
  セクタサイズ: %3

超過部分にデータは含まれていないようです

それでも続行しますか？</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>サイズが合いません！</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>照合失敗</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>セクタ %1 で検証に失敗しました。</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>イメージがデバイスより大きいため、収まる部分のみ比較できました。比較した範囲はすべて一致しましたが、デバイスには完全なイメージが入っていません。

圧縮イメージは非圧縮サイズを記録しないため、デバイスの末尾に達するまでこれを検出できませんでした。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">検証に成功しました。

イメージとデバイスの違いは GPT のみで、これは「書き込み後に GPT を修正する」オプションが意図的に書き換えたものです。</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

デバイスを取り出しました。今すぐ取り外してください。</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

デバイスを自動的にオフラインにできませんでした。</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>照合に成功しました。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>ファイルエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>ファイルのハンドルを取得しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>デバイスエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>デバイスのハンドルを取得しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>%1 を含むボリュームの空き領域を取得できませんでした。
エラー %2: %3
空き領域の確認はスキップされます。</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>ロックエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">ボリュームをロックしようとしたときにエラーが発生しました。
エラー%1：%2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>アンロックエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>ボリュームのロックを解除しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>マウント解除エラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>ボリュームをマウント解除しようとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>読み込みエラー</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>セクター数が多すぎます。</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>読み取りバッファー用のメモリを確保できませんでした。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>ハンドルからデータを読み取ろうとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>書き込みエラー</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>ハンドルにデータを書き込もうとしたときにエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>デバイスは %2 バイトのうち %1 バイトしか受け付けませんでした。デバイス上のイメージは不完全です。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>デバイスのジオメトリを取得中にエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>ファイルサイズを取得中にエラーが発生しました。
エラー %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>空き領域エラー</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">ドライブ%1の空きスペースを取得できませんでした。
エラー%2：%3
空き容量の確認はスキップされます。</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>不明なデバイス</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>このコンピューターのボリュームを一覧表示できませんでした。
エラー %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>ボリューム %1 をロックできませんでした: まだ使用中です。
デバイスを使用しているプログラムをすべて閉じて、もう一度お試しください。
エラー %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>プライマリ GPT ヘッダーのサイズが範囲外です</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>プライマリ GPT ヘッダーのチェックサムが不正です</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1、パーティションテーブルなし</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>認識できないファイルシステム、パーティションテーブルなし</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>GPT ヘッダーのサイズが範囲外です</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>GPT ヘッダーのチェックサムが不正です</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT パーティションエントリ配列がヘッダーの示す位置にありません</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA がパーティションテーブルの内側にあります</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>パーティション %1 がイメージの末尾を越えています</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>パーティション %1 の範囲が不正です</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>GPT にパーティションがありません</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>2 つのパーティションが重なっています</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>拡張 (論理パーティションを含む) (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>種類 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>MBR にパーティションがありません</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>MBR に拡張パーティションが複数あります</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>イメージが 1 セクタより小さいです</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>イメージに保護 MBR はありますが GPT ヘッダーがありません</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>拡張 MBR パーティションは GPT に配置できません: 論理パーティションのイメージを先頭領域の取得元に選択するか、除外してください</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR パーティションの種類 0x%1 には、このプログラムが知っている GPT の対応する種類がありません</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT パーティションの種類 %1 には対応する MBR の種類がありません</translation>
    </message>
    <message>
        <source>the device has no partition table to keep</source>
        <translation>デバイスには保持するパーティションテーブルがありません</translation>
    </message>
    <message>
        <source>a lead-in cannot be used while the device keeps its own partitions</source>
        <translation>デバイスが自身のパーティションを保持している間は先頭領域を使用できません</translation>
    </message>
    <message>
        <source>only a device can keep its own partitions</source>
        <translation>自身のパーティションを保持できるのはデバイスだけです</translation>
    </message>
    <message>
        <source>a free space has no size</source>
        <translation>空き領域のサイズが指定されていません</translation>
    </message>
    <message>
        <source>the device&apos;s own partitions must stay in the order they are on it</source>
        <translation>デバイス自身のパーティションは、デバイス上と同じ順番のままにする必要があります</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>パーティションが選択されていません</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>選択したパーティションが存在しません</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>パーティションが 2 回選択されています</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>パーティションテーブルがないイメージのサイズが不明です: 先にスキャンしてください</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>先頭領域のイメージにパーティションテーブルがありません</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>MBR には最大 4 個のパーティションしか入りませんが、%1 個選択されています</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>MBR に入る拡張パーティションは 1 つだけです</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>GPT に入るパーティションは %1 個までですが、%2 個選択されています</translation>
    </message>
    <message>
        <source>there is %1 MB too little free space before the device&apos;s partition %2, which is kept</source>
        <translation>保持するデバイスのパーティション %2 の前の空き領域が %1 MB 不足しています</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>レイアウトが 32 ビットの MBR エントリに収まらなくなりました</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>パーティションには %1 MB 必要ですが、デバイスは %2 MB です</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>GPT エントリ配列がデバイスに収まりません</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>GPT パーティションエントリ配列のチェックサムが不正です</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>パーティションがデバイスの末尾を超えて広がっています</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>バックアップ GPT を LBA %1 に移動しました。最終使用可能 LBA は %2 になりました。LBA %3 の古いコピーは消去しました</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>バックアップ GPT を LBA %1 に移動しました。最終使用可能 LBA は %2 になりました</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>デバイスには GPT があり、MBR はそれを写しているだけです</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>MBR に縮小先となるパーティションがありません</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>再配置後のレイアウトは 32 ビットの MBR エントリに収まりません</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>デバイスはすでにこれ以上詰められない状態です。縮小できません</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA は再配置に使用できません</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>GPT に縮小先となるパーティションがありません</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>パーティションエントリがあり得ない範囲を示しています</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>デバイスのジオメトリが使用できません</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>プライマリ GPT ヘッダーを読み取れません</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>GPT エントリ配列のジオメトリが使用できません</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>デバイスが小さすぎてエントリ配列を保持できません</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>パーティションエントリを読み取れませんでした</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>パーティションエントリが LBA 2 にないため、この機能で修復できる破損ではありません</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>修復したヘッダーを書き込めませんでした</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA を LBA 2 に戻し、ヘッダーのチェックサムを再計算しました</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>デバイスがセクタサイズ 0 を報告しています。</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>ディスク %1 を開けませんでした (エラー %2)。</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>ディスク %1 のサイズを読み取れませんでした (エラー %2)。</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>ディスク %1 のセクタは %2 バイトで、%3 バイトではありません。</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>イメージファイルを開けませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>イメージファイルのサイズを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>イメージファイルを読み取れませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>イメージファイルを先頭に戻せませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>bzip2 展開処理を開始できませんでした (bzip2 エラー %1)。</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>zstd 展開処理を開始できませんでした (zstd エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>gzip 展開処理を開始できませんでした (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>xz 展開処理を開始できませんでした (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>イメージファイルが圧縮データの途中で終わっています。切り詰められているか破損しています。</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>gzip イメージを展開できませんでした。</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>gzip イメージが破損しています (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>bzip2 イメージを展開できませんでした。</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>bzip2 イメージが破損しています (bzip2 エラー %1)。</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>zstd イメージが破損しています (zstd エラー %1)。</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>xz イメージが破損しています (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>圧縮イメージは前方向にしか読み取れません。</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>イメージファイルを作成できませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>gzip 圧縮処理を開始できませんでした (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>xz 圧縮処理を開始できませんでした (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>bzip2 圧縮処理を開始できませんでした (bzip2 エラー %1)。</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>zstd 圧縮処理を開始できませんでした (zstd エラー %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>gzip 圧縮に失敗しました (zlib エラー %1)。</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>xz 圧縮に失敗しました (lzma エラー %1)。</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>bzip2 圧縮に失敗しました (bzip2 エラー %1)。</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>zstd 圧縮に失敗しました (zstd エラー %1)。</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>イメージファイルを書き込めませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>イメージファイルは書き込み用に開かれていません。</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>イメージファイルをフラッシュできませんでした (エラー %1)。</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 は、そこに提供するパーティションの終わりより前のセクタ %2 で終わっています。イメージは不完全です。</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>ディスク %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>ソースディスクのマウントを解除します</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>読み取りの途中で変更されないよう、これらのソースディスク上のボリュームは読み取り中ロックされ、マウント解除されます:

%1

それらを使用しているプログラムは、処理が終わるまでアクセスできなくなります。ディスク上の内容は変更されません。続行しますか？</translation>
    </message>
</context>
</TS>
