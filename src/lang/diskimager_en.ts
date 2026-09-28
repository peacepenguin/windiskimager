<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>Custom Partitioning</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>Sources</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>Source / partition</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Size</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>Add images...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>Add disks...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Remove</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>Full scan</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Layout</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>Partitions, in order:</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>Up</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>Down</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>Lead-in from:</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>On the device</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>From</source>
        <translation>From</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>Write to</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>A device:</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Show all devices</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>An image file:</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation>combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Browse...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>Compress to</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>Verify after writing</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>Write...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>no device is chosen to write to</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>disk %1 could not be read</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>disk %1 has %2-byte sectors, and the sources %3-byte ones</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>Disk %1: %2</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>Save the combined image as</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>the image ends inside its own partition table</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>the partition table could not be read</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>Add images</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 cannot be used: %2.</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>Add disks</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>Tick the disks to take partitions from:</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>Also list fixed disks. The disk Windows is running from is never listed.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- the device being written to</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>Already a source.</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>Disk %1 cannot be used: %2.</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>disk</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>Scanning %1...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>%1 could not be read to the end: %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>Scanning %1: %2 read...</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>whole image</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>Partition %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>Partition %1: %2</translation>
    </message>
    <message>
        <source>%1, %2</source>
        <translation>%1, %2</translation>
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
        <translation>no partition table</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>size not recorded</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1, scanned</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>unknown: scan the image</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>None: a new, empty table</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>Tick the partitions to put on the device.</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>This cannot be written: %1.</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>Partition table (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>Lead-in</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>Backup GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1, %2 partitions: an image file of %3.</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1, %2 partitions: %3 used, %4 free of %5.</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>Images of unrecorded size are checked only when scanned or written.</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>Some partitions share a GUID: you will be asked about it.</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>Name the image file to write.</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 is one of the images being combined; choose another name.</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 already exists. Overwrite it?</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>Duplicate partition GUIDs</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>Generate new GUIDs</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>Keep them</translation>
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
        <translation>Image File</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Verify</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Device</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">Shrink image on Read</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">Read to .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">Compress the Image Read from the Device with gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">Read to .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">Compress the Image Read from the Device with xz</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">Choose partitions to read</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>Exit WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Exit Win Disk Imager</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">Check GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Check the currently selected device for GPT corruption and offer to repair it.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">Skip unpartitioned space</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>Compress during Read</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>Compress the Image Read from the Device, in the format chosen below</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>Choose Partitions to Read</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>Skip unpartitioned space on Read</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Tools</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>Check Device GPT</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>Custom Partitioning...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Image File Hash</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Hash type to generate for image file</translation>
    </message>
    <message>
        <source>None</source>
        <translation>None</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Generate selected hash on file</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Generate</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Copy hash to clipboard</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copy</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>Fix GPT after write</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Show all devices</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Progress</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Cancel current process.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Read data from &apos;Device&apos; to &apos;Image File&apos;</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Read</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Write data from &apos;Image File&apos; to &apos;Device&apos;</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Write</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Compare data in &apos;Device&apos; against &apos;Image File&apos;</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Verify the image file with the selected drive</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Verify Only</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Exit Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Exit</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>Exit?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Select a disk image</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Generating...</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>Cancel?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Write Error</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>Image file cannot be located on the target device.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Confirm overwrite</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Waiting for a task.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Exiting now will cancel verifying image.
Are you sure you want to exit?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Cancel Verify.
Are you sure you want to cancel?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>Not enough available space!</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>File Error</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>The selected file does not exist.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>The specified file contains no data.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Done.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Complete</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Write Successful.</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">Could not open the file to generate a checksum:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Please select a target device.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>Device has mounted volumes</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Write failed.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Device Error</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>The device reports a size of zero. If it is a card reader, the card may have been removed.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>Could not open the file to generate a hash:
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>Hashing...</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>Could not read the whole file to generate a hash:
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>Hashing canceled.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Write cancelled.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Clearing old partition tables...</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Could not clear the existing partition tables on the device.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>The device has been partially written and no longer holds a usable image. Write the image again before using it.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>Fixing GPT...</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Image truncated</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Write successful.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Write Successful</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">The device has been taken offline and ejected.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">The device could NOT be taken offline automatically.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">The GPT could not be fixed automatically (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">the GPT is malformed</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>Fixing the GPT failed (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>write error</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">The &quot;Fix GPT after write&quot; option is not enabled.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Remove the device now</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>You do not have permission to read the selected file.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Read failed.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Verify failed.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Verify cancelled.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Verifying...</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Partition table damaged</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken: the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation type="vanished">The device holds the image correctly, but its partition table is broken: the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Repair failed</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>The partition table could not be repaired: %1</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Image larger than device</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Disk %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Please specify an image file to use.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Scanning disks...</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>Writing: %1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>Reading: %1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>Verifying: %1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>Hashing: %1 MB/s</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">Generating checksum...</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">Could not read the whole file to generate a checksum:
%1</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">Checksum canceled.</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Please select a device.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>Could not lock the device.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>Could not open the device.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>This device&apos;s partition table is broken:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Partition table repaired.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>Partition table is still damaged.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>Partition table is valid.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Partition table</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>The GPT on this device is valid: the header and the partition entries it points at agree.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>No GPT on this device.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>This device has no GPT, so it cannot have the damage this checks for.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>Could not read the partition table.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>The partition table could not be read, or is damaged in some way other than the one this repairs.</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>The target device is also one of the sources.</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 is on the target device, and cannot be written to it.</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>The device list changed while you were confirming. Check the target device and try again.</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>Sector %1 of the device does not match sector %2 of %3.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>The device has been partially written and no longer holds a usable layout. Write it again before using it.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Writing...</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>The partition table on the device does not match what was written.</translation>
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
        <translation>Write and verify successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>Its backup is already at the end of the device, so Windows has nothing to repair.</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>Whether it boots depends on its bootloaders finding their partitions where they now are.</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>Custom Partitioning</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>The device is offline and ejected.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>The device could NOT be taken offline.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>The GPT could not be fixed (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>malformed GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>&quot;Fix GPT after write&quot; is off.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>The combined image ended early.</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>Sector %1 of %2 is not what was written.</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 holds more than the combined image, or does not end cleanly.</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>Write and verify successful.</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 holds a %2 partition table with %3 partitions from %4 images.</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">Choose Partitions</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">Partition %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">Partition %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">At least one partition must stay checked.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Read Error</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Please select a source device.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Confirm Overwrite</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Are you sure you want to overwrite the specified file?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">Read canceled.</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>Disk is not large enough for the specified image.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Reading...</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Read Canceled.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Read Successful.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>File Info</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Please specify a file to save data to.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Verify Error</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Please select a device to verify against.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>The device could not be read at sector %1.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>The device holds the image correctly, but its partition table is broken:</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>Size Mismatch!</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">Select partitions to include in the Image.</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Verify Failure</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>Verification failed at sector: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Verify Successful.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Verify Successful.

The image and the device differ only in the GPT.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

The device has been ejected. Remove it now.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

The device could NOT be taken offline automatically.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>File Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>An error occurred when attempting to get a handle on the file.
Error %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Device Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>An error occurred when attempting to get a handle on the device.
Error %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Lock Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">An error occurred when attempting to lock the volume.
Error %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Unlock Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>An error occurred when attempting to unlock the volume.
Error %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Dismount Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>An error occurred when attempting to dismount the volume.
Error %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Read Error</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>Sector count too large.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>Unable to allocate memory for read buffer.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>An error occurred when attempting to read data from handle.
Error %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Write Error</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>An error occurred when attempting to write data to handle.
Error %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>The device took only %1 of %2 bytes. The image on the device is incomplete.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>An error occurred while getting the file size.
Error %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Free Space Error</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Unknown device</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>Could not list the volumes on this computer.
Error %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>the primary GPT header size is out of range</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>the primary GPT header checksum is invalid</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1, no partition table</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>unrecognized filesystem, no partition table</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>the GPT header size is out of range</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>the GPT header checksum is invalid</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>the GPT partition entry array is not where the header says</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA lies inside the partition table</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>partition %1 runs past the end of the image</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>partition %1 describes an impossible range</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>the GPT holds no partitions</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>two partitions overlap</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>extended, with its logical partitions (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>type 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>the MBR holds no partitions</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>the MBR has more than one extended partition</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>the image is smaller than one sector</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>the image has a protective MBR but no GPT header</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR partition type 0x%1 has no GPT equivalent this program knows</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT partition type %1 has no MBR equivalent</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>no partitions are chosen</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>a chosen partition does not exist</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>a partition is chosen twice</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>the size of an image with no partition table is not known: scan it first</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>the lead-in image has no partition table</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>an MBR holds at most four partitions, and %1 are chosen</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>an MBR can hold only one extended partition</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>the GPT has room for %1 partitions, and %2 are chosen</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>the layout no longer fits a 32-bit MBR entry</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>the partitions need %1 MB and the device has %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>the GPT entry array does not fit on the device</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>the GPT partition entry array checksum is invalid</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>a partition extends past the end of the device</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>backup GPT moved to LBA %1; last usable LBA is now %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>the device has a GPT, which its MBR only mirrors</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>the MBR holds no partitions to shrink to</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>the repacked layout no longer fits a 32-bit MBR entry</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>the device is already this tight; nothing to shrink</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA is not usable for repacking</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>the GPT holds no partitions to shrink to</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>a partition entry describes an impossible range</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>the device geometry is not usable</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>the primary GPT header is not readable</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>the GPT entry array geometry is not usable</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>the device is too small to hold an entry array</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>the partition entries could not be read</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>the partition entries are not at LBA 2, so this is not the damage this can repair</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>the repaired header could not be written</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>The device reports a sector size of zero.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>Disk %1 could not be opened (error %2).</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>The size of disk %1 could not be read (error %2).</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>Disk %1 has %2-byte sectors, not %3.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>The image file could not be opened (error %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>The size of the image file could not be read (error %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>The image file could not be read (error %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>The image file could not be rewound (error %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>The bzip2 decompressor could not be started (bzip2 error %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>The zstd decompressor could not be started (zstd error %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>The gzip decompressor could not be started (zlib error %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>The xz decompressor could not be started (lzma error %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>The image file ends in the middle of the compressed data. It is truncated or damaged.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>The gzip image could not be decompressed.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>The gzip image is damaged (zlib error %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>The bzip2 image could not be decompressed.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>The bzip2 image is damaged (bzip2 error %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>The zstd image is damaged (zstd error %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>The xz image is damaged (lzma error %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>A compressed image can only be read forwards.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>The image file could not be created (error %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>The gzip compressor could not be started (zlib error %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>The xz compressor could not be started (lzma error %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>The bzip2 compressor could not be started (bzip2 error %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>The zstd compressor could not be started (zstd error %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>The gzip compressor failed (zlib error %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>The xz compressor failed (lzma error %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>The bzip2 compressor failed (bzip2 error %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>The zstd compressor failed (zstd error %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>The image file could not be written (error %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>The image file is not open for writing.</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>The image file could not be flushed (error %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>Disk %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>Source disks will be dismounted</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</translation>
    </message>
</context>
</TS>
