<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>自定义分区</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>添加映像文件或磁盘，勾选要放到设备上或新映像文件中的分区，并排列顺序。每个来源的分区表从其开头的扇区读取；在您写入或要求完整扫描之前，不会读取其他任何内容。</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>来源</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>来源 / 分区</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>添加映像...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>添加磁盘...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>也从磁盘中获取分区：存储卡、U 盘及其他磁盘。运行 Windows 的磁盘永远不会出现在可选列表中。读取某个磁盘时，其上的卷会被锁定并卸载。</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>完整扫描</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>读取并解压整个映像，以得知其确切大小，并检查它是否完整包含每个分区直至其末尾。仅在映像没有分区表且文件未记录其大小时，或要在写入前检查压缩映像时才需要。</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>布局</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>分区（按顺序）：</translation>
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
        <translation>前导区来源：</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>复制此映像的引导代码，以及其分区表与第一个分区之间的空间（最多 32 MiB），引导加载程序可能存放在那里。设备随后会获得与此映像相同类型的分区表，且第一个分区的起始位置与此映像中相同。</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>在设备上</translation>
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
        <translation>写入到</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>设备：</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>显示所有设备</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同时列出固定磁盘. 内置 PCIe 读卡器常把存储卡显示为不可移动设备, 否则将被隐藏. 运行 Windows 的磁盘永远不会被列出.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>映像文件：</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation>combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>浏览...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>压缩为</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>写入时的压缩格式：.img.zst 最快，.img.xz 最小，.img.gz 兼容性最广</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>写入后校验</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>写入...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>未选择要写入的设备</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>无法读取磁盘 %1</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>磁盘 %1 的扇区为 %2 字节，而各来源的扇区为 %3 字节</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>磁盘 %1：%2</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>将合并后的映像另存为</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>磁盘映像 (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>映像在其自身的分区表内就结束了</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>无法读取分区表</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>添加映像</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 无法使用：%2。</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 没有分区表，因此将其视为一个分区：整个映像。文件未记录其大小，因此必须读到末尾才能得知。

现在扫描吗？</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>添加磁盘</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>勾选要从中获取分区的磁盘：</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>同时列出固定磁盘. 运行 Windows 的磁盘永远不会被列出.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- 正在写入的设备</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>已是来源。</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>磁盘 %1 无法使用：%2。</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>磁盘</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>正在扫描 %1…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>无法将 %1 读到末尾：%2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>正在扫描 %1：已读取 %2…</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 在 %2 处结束，早于其分区 %3 的结束位置：映像不完整，无法完整复制该分区。</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>整个映像</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>分区 %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>分区 %1：%2</translation>
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
        <translation>无分区表</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>未记录大小</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1，已扫描</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>未知：请扫描映像</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>无：新的空分区表</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>勾选要放到设备上的分区。</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>无法写入：%1。</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>无法写入：%1 正是要写入的设备。请写入映像文件，或选择其他设备。</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>分区表 (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>前导区</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>备份 GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1，%2 个分区：映像文件大小为 %3。</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1，%2 个分区：已用 %3，%5 中剩余 %4。</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>大小未记录的映像仅在扫描或写入时才会检查。</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>部分分区的 GUID 相同：稍后会询问您如何处理。</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>请为要写入的映像文件命名。</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 是正在合并的映像之一；请选择其他名称。</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 已存在。要覆盖吗？</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 位于磁盘 %2 上，而该磁盘是来源之一：读取时其上的卷会被锁定，无法向其写入任何内容。请选择其他磁盘上的位置。</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>重复的分区 GUID</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>以下唯一分区 GUID 属于所选分区中的多个分区：

%1

这些副本通常是从同一映像的两个副本中取出的同一分区。如果 GUID 重复，通过 PARTUUID 查找分区的系统（在 fstab 或内核命令行中）可能会使用错误的分区。

可以为后面的副本生成新的 GUID；第一个保留其原有 GUID。之后，凡是通过旧 PARTUUID 引用被重新生成分区的内容都将找不到它。</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>生成新的 GUID</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>保留原样</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Win32 Disk Imager</source>
        <translation type="vanished">Win32 磁盘映像工具</translation>
    </message>
    <message>
        <source>Image File</source>
        <translation>映像文件</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>校验</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">读取时缩小映像</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">读取设备的 MBR 或 GPT，将映像缩小到仅包含实际分区。将备份 GPT 移到已用空间的末尾。</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">读取为 .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">使用 gz 压缩从设备读取的映像</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">读取为 .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">使用 xz 压缩从设备读取的映像</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">选择要读取的分区</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">在读取之前列出设备的分区，供选择要包含哪些分区。未包含的分区将从映像中移除，与未分区空间一样 -- 无论是否同时勾选了&quot;读取时缩小映像&quot;，这都会缩小映像。</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>退出 WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">退出 Win Disk Imager</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">检查 GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win 磁盘映像工具</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>检查当前所选设备的 GPT 是否损坏，若已损坏则提供修复。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">跳过未分区空间</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">读取设备的 MBR 或 GPT，并在映像中省去未分区空间，保留分区、分区表以及 GPT 在其分区之前预留的空间。备份 GPT 会移到映像新的末尾。</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>读取时压缩</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>以下方选择的格式压缩从设备读取的映像</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">读取时的压缩格式：.img.gz 生成更快，.img.xz 体积更小</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">读取设备的 MBR 或 GPT，并在映像中省去各分区之间及之后的未分区空间。第一个分区之前存放引导加载程序的空间，在分区表之后 32 MB 以内按原样读取，只省去超出的部分。备份 GPT 会移到映像新的末尾。</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>读取设备的 MBR 或 GPT，并在映像中省去各分区之间及之后的未分区空间。第一个分区之前的空间中，引导加载程序可能存放在未使用空间里，其中最多 32 MB 按原样读取，只省去超出的部分。备份 GPT 会移到映像新的末尾。</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">在读取之前列出设备的分区，供选择要包含哪些分区。未包含的分区将从映像中移除，与未分区空间一样 -- 无论是否同时勾选了“跳过未分区空间”，未分区空间也总会被跳过。</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>选择要读取的分区</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>仅读取设备的部分分区：打开“自定义分区”，以设备为来源、勾选所有分区，并以映像文件为目标。取消勾选要略去的分区。</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>读取时跳过未分区空间</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>检查设备 GPT</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>自定义分区...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>将映像文件和磁盘中的分区按您选择的顺序放到设备上或新映像文件中，并使用新的分区表。</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>映像文件哈希</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>为映像文件生成的哈希类型</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>为文件生成所选的哈希值</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>生成</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>复制哈希值到剪贴板</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>复制</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>写入后修复 GPT</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>写入后, 将备份 GPT 移到设备末尾并相应更新头部, 使 Windows 没有可&quot;修复&quot;的内容. 不勾选则改为提示您移除设备.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>显示所有设备</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>同时列出固定磁盘. 内置 PCIe 读卡器常把存储卡显示为不可移动设备, 否则将被隐藏. 运行 Windows 的磁盘永远不会被列出.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">读取设备的 MBR 或 GPT，并在映像中省去各分区之间及之后的未分区空间。第一个分区之前存放引导加载程序的部分按原样读取，第一个分区不会移动。备份 GPT 会移到映像新的末尾。</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>读取时的压缩格式：.img.zst 最快，.img.xz 最小，.img.gz 兼容性最广</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>任务进度</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>取消任务。</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>从“设备”向“映像文件”传送数据</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>读取</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>从“映像文件”向“设备”传送数据</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>写入</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>将“设备”中的数据与“映像文件”比较</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">与选择的驱动盘校验映像文件</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">仅校验</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">退出 Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>退出</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>退出?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>现在退出将造成映像文件不完整.
仍然退出?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>现在退出将造成磁盘文件不完整.
仍然退出?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>选择一个磁盘映像</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>生成中…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>取消?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>现在取消会造成目的文件不完整.
仍然取消?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>映像文件不能位于目标设备上.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>确认覆盖</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">等待任务完成。.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>现在退出将取消校验映像文件.
仍然退出?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>取消校验.
确定要取消?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>磁盘空间不足！</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>文件错误</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">写入成功, 但分区表存在风险.

%1
%2

%3

请立即物理移除该设备, 不要进行任何其他操作, 也不要将其重新插入本计算机. 请将其插入目标硬件.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>文件不存在.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>该文件为空文件.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>完成.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>写入成功.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>磁盘映像 (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>压缩磁盘映像 (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">无法打开文件以生成校验和:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>请选择目标设备.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>此设备上的所有文件和数据都将被删除。
(目标设备: %1)
确定要继续吗?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>设备上有已挂载的卷</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 在 Windows 中挂载为 %2.

该设备上所有分区中的全部内容都将被销毁且无法恢复.

请确认 %2 不是您想要保留的驱动器.

仍要写入此设备?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>写入失败。</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>设备错误</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>设备报告的容量为零. 如果这是读卡器, 存储卡可能已被取出.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>无法打开文件以计算哈希值：
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>正在计算哈希值…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>无法读取整个文件以计算哈希值：
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>哈希值计算已取消。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>映像大于设备:
  映像: 至少 %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

映像的末尾将不会被写入, 因此设备中不会包含完整的映像.

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

由于映像已压缩, 无法检查多出的空间中是否含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

多出的空间中似乎含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>所需空间超过可用空间:
  需要: %1 个扇区
  可用: %2 个扇区
  扇区大小: %3

多出的空间中似乎不含数据

仍然继续?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>已取消写入。</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>正在清除旧的分区表…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>无法清除设备上已有的分区表.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>设备仅被部分写入，不再包含可用的映像。请在使用前重新写入映像。</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>正在修复 GPT…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>映像被截断</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">写入成功.

GPT 已与设备 (%1) 保持一致, 因此 Windows 没有损坏的分区表需要修复. 可以正常移除设备.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">写入成功.

映像中不含 GPT, 因此没有分区表需要 Windows 修复. 可以正常移除设备.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>写入成功.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>写入成功</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">设备已脱机并弹出.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">无法自动将设备脱机.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">无法自动修复 GPT (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">GPT 格式有误</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>修复 GPT 失败 (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">未启用&quot;写入后修复 GPT&quot;选项.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">此映像会受 Windows GPT 重写缺陷的影响.

它在第一个分区之前保留了空间, 因此重新扫描时 Windows 会重写主分区表, 使其指向错误的扇区. 重写结果仍能通过 Windows 自身的检查, 但 Linux 会拒绝它, 设备也无法引导.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">此映像不会受 Windows GPT 重写缺陷的影响.

由于备份 GPT 不在设备末尾, 重新扫描时 Windows 仍会重写分区表, 但对于这种布局, 重写得到的值是正确的. 无论如何, 现在移除设备可使其与映像保持逐字节一致.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">无法确定此映像是否受 Windows GPT 重写缺陷的影响. 请按受影响处理: 重新扫描后, 分区表可能会被 Linux 拒绝, 设备也可能无法引导.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>请立即移除设备</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>您没有读取所选文件的权限.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">映像只能以未压缩的形式读出. 请选择不带 .gz 或 .xz 扩展名的文件名.

压缩映像 (.img.gz, .img.xz) 可以用于写入和校验.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>读取失败。</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>校验失败。</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>映像大于设备:
  映像: 至少 %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

只能校验能够容纳的部分.

仍然继续?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

由于映像已压缩, 无法检查多出的空间中是否含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>已取消校验。</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>正在校验…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>分区表已损坏</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>修复失败</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>无法修复分区表：%1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">选择要包含在映像中的分区。</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>无法读取设备的扇区 %1。</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>设备中的映像正确无误，但其分区表已损坏：</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>映像大于设备</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>设备中的映像正确无误，但其分区表仍然损坏。请勾选“写入后修复 GPT”后重新写入映像，或再次执行校验并接受修复。</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>校验成功。

设备的分区表原本已损坏，现已修复。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>校验成功。

映像与设备仅在 GPT 上存在差异，且设备上的 GPT 有效。</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>校验成功。

映像与设备仅在 GPT 上存在差异。</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[磁盘 %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>请指定要使用的映像文件.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>正在扫描磁盘…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">无法读取整个文件以生成校验和：
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>正在写入：%1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>正在读取：%1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>正在校验：%1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>正在计算哈希值：%1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">正在生成校验和…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">校验和已取消。</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1，主 GPT 头部所指向的扇区中并没有分区表项。

未启用“写入后修复 GPT”就写入存储卡时，Windows 重新扫描后便会留下这种状态。数据并未丢失，但设备将无法启动，而且大多数工具会拒绝该分区表。

现在修复分区表吗？</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>请选择设备。</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>无法锁定设备。</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>无法打开设备。</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>该设备的分区表已损坏：</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>分区表已修复。</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>分区表仍然损坏。</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>分区表有效。</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>分区表</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>该设备上的 GPT 有效：头部与其指向的分区表项一致。</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>该设备上没有 GPT。</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>该设备没有 GPT，因此不会出现本项检查所针对的损坏。</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>无法读取分区表。</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>无法读取分区表，或者其损坏方式不属于本功能所能修复的类型。</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>目标设备也是来源之一。</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 位于目标设备上，无法写入该设备。</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>确认期间设备列表发生了变化。请检查目标设备后重试。</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1：%2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 在扇区 %2 处结束，早于它要在该处提供的分区的结束位置：映像不完整。</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>设备的扇区 %1 与 %3 的扇区 %2 不一致。</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>设备仅被部分写入，不再包含可用的布局。请在使用前重新写入。</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>正在写入…</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>设备上的分区表与写入的内容不一致。</translation>
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
        <translation>写入并校验成功。

设备现有一个新的 %1 分区表，包含来自 %3 个映像的 %2 个分区。</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>写入成功。

设备现有一个新的 %1 分区表，包含来自 %3 个映像的 %2 个分区。</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>其备份已位于设备末尾，因此 Windows 没有需要修复的内容。</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>能否启动取决于其引导加载程序能否在分区现在所在的位置找到它们。</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>自定义分区</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>映像大于设备, 因此其末尾未被写入, 设备中没有完整的映像.

由于压缩映像不记录其未压缩大小, 只有在设备写满后才能发现这一点.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>写入成功。

GPT 现已与设备 (%1) 一致，Windows 没有需要修复的内容。可正常移除设备。</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>写入成功。

该映像使用 MBR 分区表而非 GPT，因此不会受 Windows GPT 重写缺陷影响。可正常移除设备。</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>写入成功。

该映像没有分区表，因此不会受 Windows GPT 重写缺陷影响。可正常移除设备。</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>设备已脱机并弹出。</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>无法将设备脱机。</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>无法修复 GPT (%1)。</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>GPT 格式错误</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>“写入后修复 GPT”未启用。</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>该映像会受影响：它在第一个分区之前预留了空间，因此重新扫描会使主分区表指向错误的扇区。Windows 仍会接受该结果，Linux 不会，设备将无法启动。</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>该映像不受影响：重新扫描仍会重写分区表，但对这种布局写入的是正确的值。现在移除设备，无论如何都与映像保持一致。</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>无法确定该映像是否受影响。请按受影响处理：重新扫描可能留下 Linux 拒绝的分区表，设备将无法启动。</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>写入成功，但分区表存在风险。

%1 %2

%3

请立即移除设备，不要再插入本机。直接插入目标硬件。</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>合并后的映像提前结束。</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>%2 的扇区 %1 与写入的内容不一致。</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 包含的内容多于合并后的映像，或未能正常结束。</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>写入并校验成功。</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 包含一个 %2 分区表，其中有来自 %4 个映像的 %3 个分区。</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>其备份 GPT 位于映像末尾；写入映像时，“写入后修复 GPT”会将其移到较大设备的末尾。</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">选择分区</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">跳过未分区空间只保留分区和分区表，以及 GPT 在其分区之前预留的空间。

某些可启动映像（例如单板计算机的映像）会把引导加载程序数据放在分区之外。以这种方式读取此类设备得到的映像可能无法启动。</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">缩小只保留分区和分区表，以及 GPT 在其分区之前预留的空间。

某些可启动映像（例如单板计算机的映像）会把引导加载程序数据放在分区之外。此类设备的缩小映像可能无法启动。</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">选择要包含在映像中的分区。未勾选的分区将被移除，与未分区空间一样。</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">分区 %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">分区 %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">必须至少保留一个分区处于勾选状态。</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>读取错误</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">映像只能以未压缩方式读回。请选择不带 .gz 或 .xz 扩展名的文件名，或勾选&quot;读取为 .img.gz&quot;或&quot;读取为 .img.xz&quot;。</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>请选择源设备.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>确认覆盖</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>确认覆盖已存在的文件?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">在设备上未找到分区表，因此没有可供选择的内容。将读取整个设备。</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">读取已取消。</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>磁盘容量不足以容纳指定的映像.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>正在读取…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>读取已取消.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>读取成功.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>文件信息</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>请选择要保存的文件路径.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>校验错误</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>请选择用于校验的设备.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

多出的空间中似乎含有数据

仍然继续?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>映像大小超过设备:
  映像: %1 个扇区
  设备: %2 个扇区
  扇区大小: %3

多出的空间中似乎不含数据

仍然继续?</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>大小不匹配!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>校验失败</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>校验失败的扇区: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>映像大于设备, 因此只能校验能够容纳的部分. 已校验的部分全部一致, 但设备中没有完整的映像.

由于压缩映像不记录其未压缩大小, 只有到达设备末尾时才能发现这一点.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">校验成功.

映像与设备仅在 GPT 上存在差异, 而&quot;写入后修复 GPT&quot;选项本就会重写这一部分.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

设备已弹出. 请立即移除.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

无法自动将设备脱机.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>校验成功.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>文件错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>获取文件句柄失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>设备错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>获取设备句柄失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>无法获取存放 %1 的卷的剩余空间.
错误 %2: %3
已忽略剩余空间检查.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>锁定错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">锁该卷时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>解锁错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>解锁该卷时失败.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>卸载错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>卸载卷时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>读取错误</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>扇区数量过大。</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>无法为读取缓冲区分配内存。</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>从该句柄中读取数据发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>写入错误</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>向该句柄写入数据时发生错误.
错误 %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>设备仅接受了 %2 个字节中的 %1 个. 设备上的映像并不完整.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>无法获取设备的磁盘几何信息.
错误 %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>无法获取文件大小.
错误 %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>剩余空间错误</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">无法得到驱动器剩余空间 %1.
错误 %2: %3
已忽略剩余空间检查.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>未知设备</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>无法列出此计算机上的卷。
错误 %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>无法锁定卷 %1: 它仍在使用中.
请关闭所有正在使用该设备的程序后重试.
错误 %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>主 GPT 头部的大小超出范围</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>主 GPT 头部的校验和无效</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1，无分区表</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>无法识别的文件系统，无分区表</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>GPT 头部的大小超出范围</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>GPT 头部的校验和无效</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>GPT 分区表项数组不在头部所指示的位置</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA 位于分区表内部</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>分区 %1 超出了映像的末尾</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>分区 %1 描述了一个不可能的范围</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>GPT 中没有分区</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>有两个分区重叠</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>扩展分区及其逻辑分区 (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>类型 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>MBR 中没有分区</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>MBR 中有多个扩展分区</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>映像小于一个扇区</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>映像有保护性 MBR，但没有 GPT 头部</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>MBR 扩展分区不能放到 GPT 上：请选择包含这些逻辑分区的映像作为前导区，或将其略去</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR 分区类型 0x%1 没有本程序已知的 GPT 对应类型</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT 分区类型 %1 没有 MBR 对应类型</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>未选择任何分区</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>所选的某个分区不存在</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>某个分区被选择了两次</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>没有分区表的映像大小未知：请先扫描</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>前导区映像没有分区表</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>MBR 最多容纳四个分区，而已选择 %1 个</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>MBR 只能容纳一个扩展分区</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>GPT 可容纳 %1 个分区，而已选择 %2 个</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>布局已超出 32 位 MBR 表项的范围</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>这些分区需要 %1 MB，而设备只有 %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>GPT 分区表项数组无法容纳于该设备</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>GPT 分区表项数组的校验和无效</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>有分区超出了设备的末尾</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>备份 GPT 已移至 LBA %1; 最后可用 LBA 现为 %2; 位于 LBA %3 的过时副本已清除</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>备份 GPT 已移至 LBA %1; 最后可用 LBA 现为 %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>设备使用 GPT，其 MBR 只是 GPT 的镜像</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>MBR 中没有可供缩小的分区</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>重新打包后的布局已超出 32 位 MBR 表项的范围</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>设备已经这么紧凑了，没有可缩小的空间</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA 不适用于重新打包</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>GPT 中没有可供缩小的分区</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>某个分区表项描述了一个不可能的范围</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>设备的磁盘几何信息不可用</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>无法读取主 GPT 头部</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>GPT 分区表项数组的几何信息不可用</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>设备太小, 无法容纳分区表项数组</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>无法读取分区表项</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>分区表项不在 LBA 2, 因此这不是本功能所能修复的损坏</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>无法写入修复后的头部</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA 已重新指向 LBA 2, 头部校验和也已重新计算</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>设备报告的扇区大小为零.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>无法打开磁盘 %1 (错误 %2)。</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>无法读取磁盘 %1 的大小 (错误 %2)。</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>磁盘 %1 的扇区为 %2 字节，而不是 %3 字节。</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>无法打开映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>无法读取映像文件的大小 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>无法读取映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>无法将映像文件回退到开头 (错误 %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>无法启动 bzip2 解压程序 (bzip2 错误 %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>无法启动 zstd 解压程序 (zstd 错误 %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>无法启动 gzip 解压程序 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>无法启动 xz 解压程序 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>映像文件在压缩数据中间结束. 它已被截断或损坏.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>无法解压该 gzip 映像.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>该 gzip 映像已损坏 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>无法解压该 bzip2 映像.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>该 bzip2 映像已损坏 (bzip2 错误 %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>该 zstd 映像已损坏 (zstd 错误 %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>该 xz 映像已损坏 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>压缩映像只能向前读取.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>无法创建映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>无法启动 gzip 压缩程序 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>无法启动 xz 压缩程序 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>无法启动 bzip2 压缩程序 (bzip2 错误 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>无法启动 zstd 压缩程序 (zstd 错误 %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>gzip 压缩失败 (zlib 错误 %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>xz 压缩失败 (lzma 错误 %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>bzip2 压缩失败 (bzip2 错误 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>zstd 压缩失败 (zstd 错误 %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>无法写入映像文件 (错误 %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>映像文件未开启以供写入。</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>无法刷新映像文件缓冲区 (错误 %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1：%2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 在扇区 %2 处结束，早于它要在该处提供的分区的结束位置：映像不完整。</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>磁盘 %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>将卸载来源磁盘</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>读取期间，这些来源磁盘上的卷会被锁定并卸载，以免中途被更改：

%1

使用这些卷的程序在运行结束前将无法访问它们。其上的内容不会被更改。是否继续？</translation>
    </message>
</context>
</TS>
