<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>自訂分割</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>加入映像檔或磁碟，勾選要放到裝置上或新映像檔中的分割區，並排列順序。每個來源的分割表會從其開頭的磁區讀取；在您寫入或要求完整掃描之前，不會讀取其他任何內容。</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>來源</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>來源 / 分割區</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>加入映像檔...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>加入磁碟...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>也從磁碟中取用分割區：記憶卡、USB 隨身碟及其他磁碟。執行 Windows 的磁碟永遠不會列入選項。讀取某個磁碟時，其上的磁碟區會被鎖定並卸載。</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>完整掃描</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>讀取並解壓縮整個映像檔，以得知其確切大小，並檢查它是否完整包含每個分割區直到其結尾。僅在映像檔沒有分割表且檔案未記錄其大小時，或要在寫入前檢查壓縮映像檔時才需要。</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>配置</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>分割區（依順序）：</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>上移</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>下移</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>前導區來源：</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>複製此映像檔的開機程式碼，以及其分割表與第一個分割區之間的空間（最多 32 MiB），開機載入程式可能存放在那裡。裝置隨後會取得與此映像檔相同類型的分割表，且第一個分割區的起始位置與此映像檔中相同。</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>在裝置上</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>起始</translation>
    </message>
    <message>
        <source>From</source>
        <translation>取自</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>寫入到</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>裝置：</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>顯示所有裝置</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同時列出固定式磁碟。內建 PCIe 讀卡機常將記憶卡顯示為不可卸除式裝置，否則便會被隱藏。執行 Windows 的磁碟永遠不會被列出。</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>映像檔：</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation>combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>瀏覽...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>壓縮為</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>寫入時的壓縮格式：.img.zst 最快，.img.xz 最小，.img.gz 相容性最廣</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>寫入後驗證</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>寫入...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>未選擇要寫入的裝置</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>無法讀取磁碟 %1</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>磁碟 %1 的磁區為 %2 位元組，而各來源的磁區為 %3 位元組</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>磁碟 %1：%2</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>將合併後的映像檔另存為</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>磁碟映像檔 (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>映像檔在其自身的分割表內就結束了</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>無法讀取分割表</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>加入映像檔</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 無法使用：%2。</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 沒有分割表，因此將其視為一個分割區：整個映像檔。檔案未記錄其大小，因此必須讀到結尾才能得知。

要立即掃描嗎？</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>加入磁碟</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>勾選要從中取用分割區的磁碟：</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>同時列出固定式磁碟。執行 Windows 的磁碟永遠不會被列出。</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- 正在寫入的裝置</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>已是來源。</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>磁碟 %1 無法使用：%2。</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>磁碟</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>正在掃描 %1…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>無法將 %1 讀到結尾：%2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>正在掃描 %1：已讀取 %2…</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 在 %2 處結束，早於其分割區 %3 的結束位置：映像檔不完整，無法完整複製該分割區。</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>整個映像檔</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>分割區 %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>分割區 %1：%2</translation>
    </message>
    <message>
        <source>%1, %2</source>
        <translation>%1，%2</translation>
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
        <translation>無分割表</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>未記錄大小</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1，已掃描</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>不明：請掃描映像檔</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>無：新的空白分割表</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>勾選要放到裝置上的分割區。</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>無法寫入：%1。</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>無法寫入：%1 正是要寫入的裝置。請寫入映像檔，或選擇其他裝置。</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>分割表 (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>前導區</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>備份 GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1，%2 個分割區：映像檔大小為 %3。</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1，%2 個分割區：已使用 %3，%5 中尚餘 %4。</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>大小未記錄的映像檔僅在掃描或寫入時才會檢查。</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>部分分割區的 GUID 相同：稍後會詢問您如何處理。</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>請為要寫入的映像檔命名。</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 是正在合併的映像檔之一；請選擇其他名稱。</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 已存在。要覆寫嗎？</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 位於磁碟 %2 上，而該磁碟是來源之一：讀取時其上的磁碟區會被鎖定，無法寫入任何內容。請選擇其他磁碟上的位置。</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>重複的分割區 GUID</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>以下唯一分割區 GUID 屬於所選分割區中的多個分割區：

%1

這些副本通常是從同一映像檔的兩個副本中取出的同一分割區。若 GUID 重複，透過 PARTUUID 尋找分割區的系統（在 fstab 或核心命令列中）可能會使用錯誤的分割區。

可以為後面的副本產生新的 GUID；第一個保留其原有 GUID。之後，凡是以舊 PARTUUID 指稱重新產生之分割區的項目都將找不到它。</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>產生新的 GUID</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>保留原樣</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation type="vanished">Win32 磁碟映像檔工具</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>映像檔</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>驗證</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>裝置</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">讀取時縮小映像檔</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">讀取裝置的 MBR 或 GPT，將映像檔縮小到僅包含實際的分割區。將備份 GPT 移到已用空間的結尾。</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">讀取為 .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">使用 gz 壓縮從裝置讀取的映像檔</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">讀取為 .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">使用 xz 壓縮從裝置讀取的映像檔</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">選擇要讀取的磁碟分割</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">讀取前先列出裝置的磁碟分割，供您選擇要納入哪些。未納入的部分會從映像檔中移除，與未分割空間相同 -- 無論是否同時勾選「讀取時縮小映像檔」，都一律會縮小映像檔。</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>離開 WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">離開 Win 磁碟映像檔工具</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">檢查 GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win 磁碟映像檔工具</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>檢查目前選取的裝置上 GPT 是否損壞，並在損壞時提供修復。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">略過未分割空間</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">讀取裝置的 MBR 或 GPT，並在映像檔中略去未分割空間，保留磁碟分割、磁碟分割表，以及 GPT 在其磁碟分割之前保留的空間。備份 GPT 會移到映像檔新的結尾。</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>讀取時壓縮</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>以下方選擇的格式壓縮從裝置讀取的映像檔</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">讀取時的壓縮格式：.img.gz 產生較快，.img.xz 檔案較小</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">讀取裝置的 MBR 或 GPT，並在映像檔中略過各分割區之間及之後的未分割空間。第一個分割區之前存放開機載入程式的空間，在分割表之後 32 MB 以內按原樣讀取，只略過超出的部分。備份 GPT 會移到映像檔新的結尾。</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>讀取裝置的 MBR 或 GPT，並在映像檔中略過各分割區之間及之後的未分割空間。第一個分割區之前的空間中，開機載入程式可能存放在未使用空間裡，其中最多 32 MB 按原樣讀取，只略過超出的部分。備份 GPT 會移到映像檔新的結尾。</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">讀取前先列出裝置的磁碟分割，供您選擇要納入哪些。未納入的部分會從映像檔中移除，與未分割空間相同 -- 無論是否同時勾選「略過未分割空間」，都一律會一併略過未分割空間。</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>選擇要讀取的分割區</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>僅讀取裝置的部分分割區：開啟「自訂分割」，以裝置為來源、勾選所有分割區，並以映像檔為目的地。取消勾選要略過的分割區。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>讀取時略過未分割空間</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>檢查裝置 GPT</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>自訂分割...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>將映像檔和磁碟中的分割區依您選擇的順序放到裝置上或新映像檔中，並使用新的分割表。</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>映像檔雜湊值</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>要為映像檔產生的雜湊類型</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>為檔案產生所選取的雜湊值</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>產生</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>複製雜湊值到剪貼簿中</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>複製</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>寫入後修正 GPT</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>寫入後，將備份 GPT 移到裝置結尾並更新標頭使其相符，讓 Windows 沒有可「修復」的項目。不勾選則改為提醒您移除裝置。</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>顯示所有裝置</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同時列出固定式磁碟。內建 PCIe 讀卡機常將記憶卡顯示為不可卸除式裝置，否則便會被隱藏。執行 Windows 的磁碟永遠不會被列出。</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">讀取裝置的 MBR 或 GPT，並在映像檔中略過各分割區之間及之後的未分割空間。第一個分割區之前存放開機載入程式的部分按原樣讀取，第一個分割區不會移動。備份 GPT 會移到映像檔新的結尾。</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>讀取時的壓縮格式：.img.zst 最快，.img.xz 最小，.img.gz 相容性最廣</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>取消目前的作業。</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>從「裝置」讀取資料到「映像檔」中</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>讀取</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>從「映像檔」寫入資料到「裝置」中</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>寫入</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>比對「裝置」中的資料與「映像檔」</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">驗證選定磁碟的映像檔</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">僅驗證</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">離開 Win32 磁碟映像檔工具</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>離開</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>離開？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致映像檔損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>現在離開將會導致磁碟損毀。
您確定要離開嗎？</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>選擇一個磁碟映像檔</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>產生中…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>取消？</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>現在取消將會導致目標檔案損毀。
您確定要取消嗎？</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>映像檔不能位於目標裝置上。</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>確認覆寫</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">等待工作。</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>現在離開將會取消驗證映像檔。
您確定要離開嗎？</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>取消驗證。
您確定要取消嗎？</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>沒有足夠的可用空間！</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">寫入成功，但分割表有風險。

%1
%2

%3

請立刻將裝置實體移除，在此之前不要進行任何其他操作，也不要將它重新插入這台電腦。請將它插入目標硬體。</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>選取的檔案不存在。</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>指定的檔案不包含資料。</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>完成。</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>寫入成功。</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>磁碟映像檔 (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>壓縮磁碟映像檔 (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>錯誤</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">無法開啟檔案以產生總和檢查碼：
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>請選擇目標裝置。</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>此裝置上的所有檔案和資料都將被刪除。
(目標裝置：%1)
您確定要繼續嗎？</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>裝置有已掛載的磁碟區</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 在 Windows 中掛載為 %2。

此裝置上所有分割區中的一切都將被銷毀且無法復原。

請確認 %2 不是您想要保留的磁碟機。

仍要寫入此裝置嗎？</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>寫入失敗。</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>裝置錯誤</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>裝置回報的大小為零。若這是讀卡機，記憶卡可能已被取出。</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>無法開啟檔案以計算雜湊值：
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>正在計算雜湊值…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>無法讀取整個檔案以計算雜湊值：
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>雜湊值計算已取消。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>映像檔大於裝置：
  映像檔：至少 %1 個磁區
  可用：%2 個磁區
  磁區大小：%3

映像檔的結尾將不會被寫入，因此裝置中不會包含完整的映像檔。

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

由於映像檔已壓縮，無法檢查多出的空間中是否含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

多出的空間中似乎含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>所需空間超過可用空間：
  需要：%1 個磁區
  可用：%2 個磁區
  磁區大小：%3

多出的空間中似乎不含資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>已取消寫入。</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>正在清除舊的分割表…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>無法清除裝置上既有的分割表。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>裝置僅被部分寫入，不再包含可用的映像檔。請在使用前重新寫入映像檔。</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>正在修正 GPT…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>映像檔遭截斷</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">寫入成功。

GPT 已與裝置 (%1) 保持一致，因此 Windows 沒有損壞的分割表需要修復。可以正常移除裝置。</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">寫入成功。

映像檔中不含 GPT，因此沒有分割表需要 Windows 修復。可以正常移除裝置。</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>寫入成功。</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>寫入成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">裝置已離線並退出。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">無法自動將裝置離線。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">無法自動修正 GPT (%1)。</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT 格式不正確</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>修正 GPT 失敗 (%1)。</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">未啟用「寫入後修正 GPT」選項。</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">此映像檔會受到 Windows GPT 重寫錯誤的影響。

它在第一個分割區之前保留了空間，因此重新掃描時 Windows 會重寫主要分割表，使其指向錯誤的磁區。重寫的結果仍能通過 Windows 自身的檢查，但 Linux 會拒絕它，裝置也無法開機。</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">此映像檔不會受到 Windows GPT 重寫錯誤的影響。

由於備份 GPT 不在裝置結尾，重新掃描時 Windows 仍會重寫分割表，但以這種配置而言，重寫得到的值是正確的。無論如何，現在移除裝置都能讓它與映像檔保持逐位元組相同。</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">無法判斷此映像檔是否受到 Windows GPT 重寫錯誤的影響。請假設它會受影響：重新掃描可能導致分割表被 Linux 拒絕，裝置也無法開機。</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>請立即移除裝置</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>您沒有讀取所選檔案的權限。</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">映像檔只能以未壓縮的形式讀出。請選擇不帶 .gz 或 .xz 副檔名的檔案名稱。

壓縮映像檔 (.img.gz、.img.xz) 則可以用於寫入與驗證。</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>讀取失敗。</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>驗證失敗。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>映像檔大於裝置：
  映像檔：至少 %1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

只能比對放得下的部分。

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

由於映像檔已壓縮，無法檢查多出的空間中是否含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>已取消驗證。</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>正在驗證…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>分割表已損壞</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>修復失敗</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>無法修復分割表：%1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">選擇要納入映像檔的磁碟分割。</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>無法讀取裝置的磁區 %1。</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>裝置中的映像檔正確無誤，但其分割表已損壞：</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>映像檔大於裝置</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>裝置中的映像檔正確無誤，但其分割表仍然損壞。請勾選「寫入後修正 GPT」後重新寫入映像檔，或再次執行驗證並接受修復。</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>驗證成功。

裝置的分割表原本已損壞，現已修復。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>驗證成功。

映像檔與裝置僅在 GPT 上有差異，且裝置上的 GPT 有效。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>驗證成功。

映像檔與裝置僅在 GPT 上有差異。</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[磁碟 %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>請指定所要使用的映像檔。</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>正在掃描磁碟…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">無法讀取整個檔案以產生總和檢查碼：
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>正在寫入：%1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>正在讀取：%1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>正在驗證：%1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>正在計算雜湊值：%1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">正在產生校驗和…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">校驗和已取消。</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1，主要 GPT 標頭指向的磁區並沒有分割區項目。

未啟用「寫入後修正 GPT」就寫入記憶卡時，Windows 重新掃描後便會留下這種狀態。資料並未遺失，但裝置將無法開機，而且大多數工具會拒絕這個分割表。

要立即修復分割表嗎？</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>請選擇裝置。</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>無法鎖定裝置。</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>無法開啟裝置。</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>此裝置的分割表已損壞：</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>分割表已修復。</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>分割表仍然損壞。</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>分割表有效。</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>分割表</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>此裝置上的 GPT 有效：標頭與其指向的分割區項目一致。</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>此裝置上沒有 GPT。</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>此裝置沒有 GPT，因此不會發生這項檢查所針對的損壞。</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>無法讀取分割表。</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>無法讀取分割表，或其損壞方式不屬於這項功能所能修復的類型。</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>目標裝置也是來源之一。</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 位於目標裝置上，無法寫入該裝置。</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>確認期間裝置清單已變更。請檢查目標裝置後再試一次。</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1：%2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 在磁區 %2 處結束，早於它要在該處提供的分割區的結束位置：映像檔不完整。</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>裝置的磁區 %1 與 %3 的磁區 %2 不相符。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>裝置僅被部分寫入，不再包含可用的配置。請在使用前重新寫入。</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>正在寫入…</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>裝置上的分割表與寫入的內容不相符。</translation>
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
        <source>Write and verify successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>寫入並驗證成功。

裝置現有一個新的 %1 分割表，包含來自 %3 個映像檔的 %2 個分割區。</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>寫入成功。

裝置現有一個新的 %1 分割表，包含來自 %3 個映像檔的 %2 個分割區。</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>其備份已位於裝置結尾，因此 Windows 沒有需要修復的項目。</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>能否開機取決於其開機載入程式能否在分割區目前所在的位置找到它們。</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>自訂分割</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>映像檔大於裝置，因此其結尾未被寫入，裝置中沒有完整的映像檔。

由於壓縮映像檔不會記錄其未壓縮大小，直到裝置寫滿時才能發現這一點。</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>寫入成功。

GPT 現已與裝置 (%1) 一致，Windows 沒有需要修復的項目。可正常移除裝置。</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>寫入成功。

此映像檔使用 MBR 分割表而非 GPT，因此不會受 Windows GPT 重寫缺陷影響。可正常移除裝置。</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>寫入成功。

此映像檔沒有分割表，因此不會受 Windows GPT 重寫缺陷影響。可正常移除裝置。</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>裝置已離線並退出。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>無法將裝置離線。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>無法修正 GPT (%1)。</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>GPT 格式錯誤</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>「寫入後修正 GPT」未啟用。</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>此映像檔會受影響：它在第一個分割區之前保留了空間，因此重新掃描會使主分割表指向錯誤的磁區。Windows 仍會接受該結果，Linux 不會，裝置將無法開機。</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>此映像檔不受影響：重新掃描仍會重寫分割表，但對這種配置寫入的是正確的值。現在移除裝置，無論如何都與映像檔保持一致。</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>無法確定此映像檔是否受影響。請按受影響處理：重新掃描可能留下 Linux 拒絕的分割表，裝置將無法開機。</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>寫入成功，但分割表存在風險。

%1 %2

%3

請立即移除裝置，不要再插入本機。直接插入目標硬體。</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>合併後的映像檔提前結束。</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>%2 的磁區 %1 與寫入的內容不相符。</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 包含的內容多於合併後的映像檔，或未能正常結束。</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>寫入並驗證成功。</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 包含一個 %2 分割表，其中有來自 %4 個映像檔的 %3 個分割區。</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>其備份 GPT 位於映像檔結尾；寫入映像檔時，「寫入後修正 GPT」會將其移到較大裝置的結尾。</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">選擇磁碟分割</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">略過未分割空間只會保留磁碟分割與磁碟分割表，以及 GPT 在其磁碟分割之前保留的空間。

部分可開機映像檔（例如單板電腦的映像檔）會把開機載入程式的資料放在磁碟分割之外。以這種方式讀取這類裝置所得的映像檔可能無法開機。</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">縮小只會保留磁碟分割與磁碟分割表，以及 GPT 在其磁碟分割之前保留的空間。

部分可開機映像檔（例如單板電腦的映像檔）會把開機載入程式的資料放在磁碟分割之外。這類裝置縮小後的映像檔可能無法開機。</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">選擇要納入映像檔的磁碟分割。未勾選的部分會被移除，與未分割空間相同。</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">磁碟分割 %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">磁碟分割 %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">必須至少保留一個磁碟分割處於勾選狀態。</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>讀取錯誤</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">映像檔只能以未壓縮方式讀回。請選擇不含 .gz 或 .xz 副檔名的檔名，或勾選「讀取為 .img.gz」或「讀取為 .img.xz」。</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>請選擇來源裝置。</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>確認覆寫</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>您確定要覆寫指定的檔案嗎？</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">裝置上未找到磁碟分割表，因此沒有可供選擇的內容。將讀取整個裝置。</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">讀取已取消。</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>磁碟空間不足以放置指定的映像檔。</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>正在讀取…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>讀取已取消。</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>讀取成功。</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>檔案資訊</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>請指定要儲存資料到哪一個檔案。</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>驗證錯誤</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>請選擇要用於驗證的裝置。</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

多出的空間中似乎含有資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>映像檔大小超過裝置：
  映像檔：%1 個磁區
  裝置：%2 個磁區
  磁區大小：%3

多出的空間中似乎不含資料

仍要繼續嗎？</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>大小不符合！</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>驗證失敗</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>驗證失敗，磁區：%1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>映像檔大於裝置，因此只能比對放得下的部分。已比對的部分完全相符，但裝置中沒有完整的映像檔。

由於壓縮映像檔不會記錄其未壓縮大小，直到到達裝置結尾時才能發現這一點。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">驗證成功。

映像檔與裝置僅在 GPT 上有差異，而「寫入後修正 GPT」選項本來就會重寫這個部分。</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

裝置已退出。請立即移除。</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

無法自動將裝置離線。</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>驗證成功。</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>檔案錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>嘗試取得檔案控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>裝置錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>嘗試取得裝置控制代碼失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>無法取得存放 %1 的磁碟區剩餘空間。
錯誤 %2：%3
將會跳過剩餘空間檢查。</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>鎖定錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">嘗試鎖定該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>解除鎖定錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>嘗試解除鎖定該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>卸載錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>嘗試卸載該卷冊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>讀取錯誤</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>磁區數量過大。</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>無法為讀取緩衝區配置記憶體。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>嘗試從該控制代碼讀取資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>寫入錯誤</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>嘗試向該控制代碼寫入資料時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>裝置只接受了 %2 個位元組中的 %1 個。裝置上的映像檔並不完整。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>嘗試取得該裝置的磁碟結構資訊時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>嘗試取得該檔案大小時失敗。
錯誤 %1：%2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>剩餘空間錯誤</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">無法取得磁碟 %1 的剩餘空間。
錯誤 %2：%3
將會跳過剩餘空間檢查。</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>不明的裝置</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>無法列出此電腦上的磁碟區。
錯誤 %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>無法鎖定磁碟區 %1：它仍在使用中。
請關閉所有正在使用該裝置的程式後再試一次。
錯誤 %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>主要 GPT 標頭的大小超出範圍</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>主要 GPT 標頭的總和檢查碼無效</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1，無分割表</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>無法辨識的檔案系統，無分割表</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>GPT 標頭的大小超出範圍</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>GPT 標頭的總和檢查碼無效</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT 分割區項目陣列不在標頭所指的位置</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA 位於分割表內部</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>分割區 %1 超出了映像檔的結尾</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>分割區 %1 描述了一個不可能的範圍</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>GPT 中沒有分割區</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>有兩個分割區重疊</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>延伸分割區及其邏輯分割區 (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>類型 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>MBR 中沒有分割區</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>MBR 中有多個延伸分割區</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>映像檔小於一個磁區</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>映像檔有保護性 MBR，但沒有 GPT 標頭</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>MBR 延伸分割區無法放到 GPT 上：請選擇包含這些邏輯分割區的映像檔作為前導區，或將其略過</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR 分割區類型 0x%1 沒有本程式已知的 GPT 對應類型</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT 分割區類型 %1 沒有 MBR 對應類型</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>未選擇任何分割區</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>所選的某個分割區不存在</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>某個分割區被選擇了兩次</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>沒有分割表的映像檔大小不明：請先掃描</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>前導區映像檔沒有分割表</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>MBR 最多容納四個分割區，而已選擇 %1 個</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>MBR 只能容納一個延伸分割區</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>GPT 可容納 %1 個分割區，而已選擇 %2 個</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>配置已超出 32 位元 MBR 項目的範圍</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>這些分割區需要 %1 MB，而裝置只有 %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>GPT 項目陣列放不進裝置</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>GPT 分割區項目陣列的總和檢查碼無效</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>有分割區超出了裝置的結尾</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>備份 GPT 已移至 LBA %1；最後可用的 LBA 現在是 %2；位於 LBA %3 的過時副本已清除</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>備份 GPT 已移至 LBA %1；最後可用的 LBA 現在是 %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>裝置使用 GPT，其 MBR 只是 GPT 的鏡像</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>MBR 中沒有可供縮小的分割區</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>重新排列後的配置已超出 32 位元 MBR 項目的範圍</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>裝置已經這麼緊湊了，沒有可縮小的空間</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA 不適用於重新排列</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>GPT 中沒有可供縮小的分割區</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>某個分割區項目描述了一個不可能的範圍</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>裝置的磁碟結構資訊無法使用</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>無法讀取主要 GPT 標頭</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>GPT 項目陣列的結構資訊無法使用</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>裝置太小，無法容納項目陣列</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>無法讀取分割區項目</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>分割區項目不在 LBA 2，因此這不是本功能所能修復的損壞</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>無法寫入修復後的標頭</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA 已重新指向 LBA 2，標頭的總和檢查碼也已重新計算</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>裝置回報的磁區大小為零。</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>無法開啟磁碟 %1 (錯誤 %2)。</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>無法讀取磁碟 %1 的大小 (錯誤 %2)。</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>磁碟 %1 的磁區為 %2 位元組，而不是 %3 位元組。</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>無法開啟映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>無法讀取映像檔的大小 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>無法讀取映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>無法將映像檔倒回開頭 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>無法啟動 bzip2 解壓縮程式 (bzip2 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>無法啟動 zstd 解壓縮程式 (zstd 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>無法啟動 gzip 解壓縮程式 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>無法啟動 xz 解壓縮程式 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>映像檔在壓縮資料的中間結束。它已被截斷或損毀。</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>無法解壓縮此 gzip 映像檔。</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>此 gzip 映像檔已損毀 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>無法解壓縮此 bzip2 映像檔。</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>此 bzip2 映像檔已損毀 (bzip2 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>此 zstd 映像檔已損毀 (zstd 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>此 xz 映像檔已損毀 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>壓縮映像檔只能向前讀取。</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>無法建立映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>無法啟動 gzip 壓縮程式 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>無法啟動 xz 壓縮程式 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>無法啟動 bzip2 壓縮程式 (bzip2 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>無法啟動 zstd 壓縮程式 (zstd 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>gzip 壓縮失敗 (zlib 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>xz 壓縮失敗 (lzma 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>bzip2 壓縮失敗 (bzip2 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>zstd 壓縮失敗 (zstd 錯誤 %1)。</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>無法寫入映像檔 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>映像檔未開啟以供寫入。</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>無法清空映像檔緩衝區 (錯誤 %1)。</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1：%2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 在磁區 %2 處結束，早於它要在該處提供的分割區的結束位置：映像檔不完整。</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>磁碟 %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>將卸載來源磁碟</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>讀取期間，這些來源磁碟上的磁碟區會被鎖定並卸載，以免中途遭到變更：

%1

使用這些磁碟區的程式在作業結束前將無法存取它們。其上的內容不會被變更。要繼續嗎？</translation>
    </message>
</context>
</TS>
