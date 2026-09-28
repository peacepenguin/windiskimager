<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>Benutzerdefinierte Partitionierung</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>Fügen Sie Image-Dateien oder Datenträger hinzu, haken Sie die Partitionen an, die auf den Datenträger oder in eine neue Image-Datei kommen sollen, und ordnen Sie sie an. Die Partitionstabelle jeder Quelle wird aus ihren ersten Sektoren gelesen; alles andere wird erst beim Schreiben oder bei einem vollständigen Scan gelesen.</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>Quellen</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>Quelle / Partition</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>Images hinzufügen...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>Datenträger hinzufügen...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>Partitionen auch von Datenträgern übernehmen: Speicherkarten, USB-Laufwerke und andere Datenträger. Der Datenträger, von dem Windows läuft, wird nie angeboten. Während ein Datenträger gelesen wird, sind seine Volumes gesperrt und ausgehängt.</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>Vollständiger Scan</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>Liest und dekomprimiert das ganze Image, um seine genaue Größe zu ermitteln und zu prüfen, dass es jede Partition bis zu ihrem Ende enthält. Nur nötig für ein Image ohne Partitionstabelle, dessen Größe die Datei nicht angibt, oder um ein komprimiertes Image vor dem Schreiben zu prüfen.</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Aufteilung</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>Partitionen, in Reihenfolge:</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>Nach oben</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>Nach unten</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>Vorspann von:</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>Kopiert den Bootcode dieses Images und den Bereich zwischen seiner Partitionstabelle und seiner ersten Partition (bis zu 32 MiB), in dem ein Bootloader liegen kann. Der Datenträger erhält dann dieselbe Art von Partitionstabelle wie dieses Image, und die erste Partition beginnt dort, wo sie in diesem Image begann.</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>Auf dem Datenträger</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Beginn</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Von</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>Schreiben auf</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>Einen Datenträger:</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Alle Datenträger anzeigen</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Listet auch fest eingebaute Datenträger auf. Interne PCIe-Kartenleser melden die Karte oft als nicht wechselbares Gerät, das sonst ausgeblendet bleibt. Der Datenträger, von dem Windows läuft, wird nie aufgeführt.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>Eine Image-Datei:</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation>combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Durchsuchen...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>Komprimieren als</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Das komprimierte Format zum Schreiben: .img.zst ist am schnellsten, .img.xz am kleinsten und .img.gz am weitesten unterstützt</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>Nach dem Schreiben prüfen</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>Schreiben...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>es ist kein Zieldatenträger gewählt</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>Datenträger %1 konnte nicht gelesen werden</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>Datenträger %1 hat %2-Byte-Sektoren, die Quellen %3-Byte-Sektoren</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>Datenträger %1: %2</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>Kombiniertes Image speichern unter</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>Datenträger-Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>das Image endet innerhalb seiner eigenen Partitionstabelle</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>die Partitionstabelle konnte nicht gelesen werden</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>Images hinzufügen</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 kann nicht verwendet werden: %2.</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 hat keine Partitionstabelle und wird daher als eine Partition behandelt: das ganze Image. Die Datei gibt nicht an, wie groß es ist, daher muss es bis zum Ende gelesen werden, um das zu ermitteln.

Jetzt scannen?</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>Datenträger hinzufügen</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>Haken Sie die Datenträger an, von denen Partitionen übernommen werden sollen:</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>Listet auch fest eingebaute Datenträger auf. Der Datenträger, von dem Windows läuft, wird nie aufgeführt.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- der Datenträger, auf den geschrieben wird</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>Bereits eine Quelle.</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>Datenträger %1 kann nicht verwendet werden: %2.</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>Datenträger</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>%1 wird gescannt...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>%1 konnte nicht bis zum Ende gelesen werden: %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>%1 wird gescannt: %2 gelesen...</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 endet bei %2, vor dem Ende seiner Partition %3: das Image ist unvollständig, und diese Partition kann nicht vollständig kopiert werden.</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>ganzes Image</translation>
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
        <translation>keine Partitionstabelle</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>Größe nicht angegeben</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1, gescannt</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>unbekannt: Image scannen</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>Keine: eine neue, leere Tabelle</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>Haken Sie die Partitionen an, die auf den Datenträger kommen sollen.</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>Dies kann nicht geschrieben werden: %1.</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>Dies kann nicht geschrieben werden: %1 ist der Datenträger, auf den geschrieben wird. Schreiben Sie in eine Image-Datei, oder wählen Sie einen anderen Datenträger.</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>Partitionstabelle (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>Vorspann</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>Sicherungs-GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1, %2 Partitionen: eine Image-Datei von %3.</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1, %2 Partitionen: %3 belegt, %4 frei von %5.</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>Images mit nicht angegebener Größe werden erst beim Scannen oder Schreiben geprüft.</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>Einige Partitionen haben dieselbe GUID: Sie werden dazu gefragt.</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>Geben Sie den Namen der zu schreibenden Image-Datei an.</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 ist eines der zu kombinierenden Images; wählen Sie einen anderen Namen.</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 ist bereits vorhanden. Überschreiben?</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 liegt auf Datenträger %2, der eine der Quellen ist: seine Volumes sind gesperrt, während er gelesen wird, daher kann nichts auf sie geschrieben werden. Wählen Sie einen Ort auf einem anderen Datenträger.</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>Doppelte Partitions-GUIDs</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>Diese eindeutigen Partitions-GUIDs gehören zu mehr als einer der gewählten Partitionen:

%1

Meist handelt es sich um dieselbe Partition aus zwei Kopien eines Images. Bei doppelten GUIDs kann ein System, das seine Partitionen über PARTUUID findet -- in fstab oder auf der Kernel-Befehlszeile --, die falsche verwenden.

Für die späteren Kopien können neue GUIDs erzeugt werden; die erste behält ihre eigene. Alles, was eine neu erzeugte Partition über ihre alte PARTUUID benennt, findet sie danach nicht mehr.</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>Neue GUIDs erzeugen</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>Beibehalten</translation>
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
        <translation>Image-Datei</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Prüfen</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Datenträger</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">Image beim Lesen verkleinern</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">Liest den MBR oder die GPT des Datenträgers und verkleinert das Image so, dass es nur die tatsächlichen Partitionen umfasst. Verschiebt die Sicherungs-GPT an das Ende des belegten Speicherbereichs.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">Als .img.gz lesen</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">Komprimiert das vom Gerät gelesene Image mit gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">Als .img.xz lesen</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">Komprimiert das vom Gerät gelesene Image mit xz</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">Zu lesende Partitionen auswählen</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">Listet vor dem Lesen die Partitionen des Datenträgers auf und lässt auswählen, welche einbezogen werden. Alles Ausgelassene wird aus dem Image entfernt, genau wie nicht partitionierter Speicherplatz -- dies verkleinert das Image immer, unabhängig davon, ob „Image beim Lesen verkleinern“ auch aktiviert ist.</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>WinDiskImager beenden</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager beenden</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">GPT prüfen</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Prüft den aktuell ausgewählten Datenträger auf eine beschädigte GPT und bietet an, sie zu reparieren.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">Nicht partitionierten Bereich überspringen</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Liest den MBR oder die GPT des Datenträgers und lässt den nicht partitionierten Speicherplatz aus dem Image weg. Erhalten bleiben die Partitionen, die Partitionstabelle und jeder Bereich, den eine GPT vor ihren Partitionen reserviert. Die Sicherungs-GPT wird an das neue Ende des Images verschoben.</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>Beim Lesen komprimieren</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>Komprimiert das vom Gerät gelesene Image im darunter gewählten Format</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">Das komprimierte Format zum Lesen: .img.gz ist schneller erstellt, .img.xz ist kleiner</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Liest den MBR oder die GPT des Datenträgers und lässt den nicht partitionierten Speicherplatz zwischen und nach seinen Partitionen weg. Der Bereich vor der ersten Partition, in dem ein Bootloader liegt, wird bis 32 MB nach der Partitionstabelle unverändert gelesen; nur was darüber hinausgeht, wird weggelassen. Die Sicherungs-GPT wird an das neue Ende des Images verschoben.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>Liest den MBR oder die GPT des Datenträgers und lässt den nicht partitionierten Speicherplatz zwischen und nach seinen Partitionen weg. Bis zu 32 MB des Bereichs vor der ersten Partition, in dem ein Bootloader im ungenutzten Speicherplatz liegen kann, werden unverändert gelesen; nur was darüber hinausgeht, wird weggelassen. Die Sicherungs-GPT wird an das neue Ende des Images verschoben.</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">Listet vor dem Lesen die Partitionen des Datenträgers auf und lässt auswählen, welche einbezogen werden. Alles Ausgelassene wird aus dem Image entfernt, genau wie nicht partitionierter Speicherplatz -- dabei wird nicht partitionierter Speicherplatz immer mit übersprungen, unabhängig davon, ob „Nicht partitionierten Bereich überspringen“ auch aktiviert ist.</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>Zu lesende Partitionen wählen</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>Nur einige Partitionen des Datenträgers lesen: öffnet „Benutzerdefinierte Partitionierung“ mit dem Datenträger als Quelle, allen Partitionen angehakt und der Image-Datei als Ziel. Entfernen Sie die Haken bei allem, was ausgelassen werden soll.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>Nicht partitionierten Bereich beim Lesen überspringen</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>GPT des Datenträgers prüfen</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>Benutzerdefinierte Partitionierung...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>Partitionen aus Image-Dateien und von Datenträgern in einer frei gewählten Reihenfolge unter einer neuen Partitionstabelle auf einen Datenträger oder in eine neue Image-Datei bringen.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Prüfsumme der Image-Datei</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Prüfsummentyp, der für die Image-Datei berechnet wird</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Ausgewählten Hash-Wert für Datei erzeugen</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Hash-Wert in Zwischenablage kopieren</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>GPT nach dem Schreiben reparieren</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Verschiebt die Sicherungs-GPT nach dem Schreiben an das Ende des Datenträgers und passt den Header an, sodass Windows nichts zu „reparieren“ hat. Ohne Häkchen wird stattdessen eine Warnung angezeigt, den Datenträger zu entfernen.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Alle Datenträger anzeigen</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Listet auch fest eingebaute Datenträger auf. Interne PCIe-Kartenleser melden die Karte oft als nicht wechselbares Gerät, das sonst ausgeblendet bleibt. Der Datenträger, von dem Windows läuft, wird nie aufgeführt.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Liest den MBR oder die GPT des Datenträgers und lässt den nicht partitionierten Speicherplatz zwischen und nach seinen Partitionen weg. Alles vor der ersten Partition, wo ein Bootloader liegt, wird unverändert gelesen, und die erste Partition wird nicht verschoben. Die Sicherungs-GPT wird an das neue Ende des Images verschoben.</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Das komprimierte Format zum Lesen: .img.zst ist am schnellsten, .img.xz am kleinsten und .img.gz am weitesten unterstützt</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Fortschritt</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Aktuellen Vorgang abbrechen.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Vom Datenträger lesen und als Image-Datei schreiben</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Lesen</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Image-Datei auf den Datenträger schreiben</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Schreiben</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Datenträger mit der Image-Datei vergleichen</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Image-Datei mit ausgewähltem Datenträger vergleichen</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Nur prüfen</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Win32 Disk Imager beenden</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>Beenden?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einer beschädigten Image-Datei.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt das Programm beenden, führt das zu einem beschädigten Datenträger.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Wählen Sie eine Image-Datei aus</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Wird berechnet …</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>Abbrechen?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Wenn Sie jetzt abbrechen, führt das zu einem beschädigten Ziel.
Sind Sie sicher, dass Sie jetzt abbrechen möchten?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>Image-Datei kann nicht auf dem Zielgerät gefunden werden.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Warte auf etwas zu tun.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Wenn Sie jetzt beenden, wird die Überprüfung des Images abgebrochen.
Sind Sie sicher, dass Sie beenden möchten?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Überprüfung abbrechen.
Sind Sie sicher, dass Sie abbrechen möchten?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>Nicht genug verfügbarer Speicherplatz!</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>Dateifehler</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Schreiben erfolgreich, aber die Partitionstabelle ist gefährdet.

%1
%2

%3

Entfernen Sie den Datenträger JETZT physisch, bevor Sie irgendetwas anderes tun, und stecken Sie ihn nicht wieder in diesen Computer. Stecken Sie ihn stattdessen in die Zielhardware.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>Die ausgewählte Datei ist nicht vorhanden.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>Die angegebene Datei enthält keine Daten.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Erledigt.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Abgeschlossen</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Schreiben war erfolgreich.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>Datenträger-Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>Komprimierte Datenträger-Images (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">Die Datei konnte zum Berechnen der Prüfsumme nicht geöffnet werden:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Bitte wählen Sie einen Zieldatenträger aus.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Alle Dateien und Daten auf diesem Gerät werden gelöscht.
(Zieldatenträger: %1)
Möchten Sie wirklich fortfahren?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>Der Datenträger hat eingebundene Volumes</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 ist in Windows als %2 eingebunden.

Alles auf diesem Datenträger, auf jeder seiner Partitionen, wird unwiederbringlich zerstört.

Prüfen Sie, dass %2 kein Laufwerk ist, das Sie behalten wollten.

Trotzdem auf diesen Datenträger schreiben?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Schreiben fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Datenträgerfehler</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>Der Datenträger meldet eine Größe von null. Falls es sich um einen Kartenleser handelt, wurde die Karte möglicherweise entfernt.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>Die Datei konnte nicht geöffnet werden, um einen Hashwert zu berechnen:
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>Hashwert wird berechnet …</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>Die Datei konnte nicht vollständig gelesen werden, um einen Hashwert zu berechnen:
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>Hashberechnung abgebrochen.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: mindestens %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Das Ende des Images wird nicht geschrieben, der Datenträger enthält dann kein vollständiges Image.

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich konnte nicht auf Daten geprüft werden, weil das Image komprimiert ist

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint Daten zu ENTHALTEN

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Es wird mehr Speicherplatz benötigt als verfügbar ist:
  Benötigt: %1 Sektoren
  Verfügbar: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint keine Daten zu enthalten

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Schreiben abgebrochen.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Alte Partitionstabellen werden gelöscht …</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Die vorhandenen Partitionstabellen auf dem Datenträger konnten nicht gelöscht werden.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>Der Datenträger wurde nur teilweise beschrieben und enthält kein verwendbares Image mehr. Schreiben Sie das Image erneut, bevor Sie den Datenträger verwenden.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>GPT wird repariert …</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Image abgeschnitten</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">Das Image ist größer als der Datenträger, daher wurde sein Ende nicht geschrieben und der Datenträger enthält kein vollständiges Image.

Dies konnte erst festgestellt werden, als der Datenträger voll war, weil ein gzip-Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Schreiben erfolgreich.

Die GPT wurde mit dem Datenträger in Einklang gebracht (%1), sodass Windows keine beschädigte Tabelle zu reparieren hat. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Schreiben erfolgreich.

Das Image enthält keine GPT, also gibt es für Windows keine Partitionstabelle zu reparieren. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Schreiben erfolgreich.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Schreiben erfolgreich</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">Der Datenträger wurde offline geschaltet und ausgeworfen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">Der Datenträger konnte NICHT automatisch offline geschaltet werden.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">Die GPT konnte nicht automatisch repariert werden (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">die GPT ist fehlerhaft aufgebaut</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>Das Reparieren der GPT ist fehlgeschlagen (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>Schreibfehler</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">Die Option „GPT nach dem Schreiben reparieren“ ist nicht aktiviert.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">Dieses Image IST vom GPT-Rewrite-Fehler von Windows betroffen.

Es reserviert Platz vor seiner ersten Partition, daher schreibt Windows bei einem erneuten Einlesen die primäre Partitionstabelle so um, dass sie auf die falschen Sektoren zeigt. Das Ergebnis besteht die Prüfungen von Windows weiterhin, wird von Linux aber abgelehnt, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">Dieses Image ist NICHT vom GPT-Rewrite-Fehler von Windows betroffen.

Windows schreibt die Tabelle bei einem erneuten Einlesen trotzdem um, weil die Sicherungs-GPT nicht am Ende des Datenträgers liegt, aber bei diesem Layout trifft die Neuberechnung die richtigen Werte. Wenn Sie den Datenträger jetzt entfernen, bleibt er in jedem Fall Byte für Byte mit dem Image identisch.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">Ob dieses Image vom GPT-Rewrite-Fehler von Windows betroffen ist, konnte nicht ermittelt werden. Gehen Sie davon aus, dass es betroffen ist: Ein erneutes Einlesen kann dazu führen, dass Linux die Partitionstabelle ablehnt und der Datenträger nicht mehr startet.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Entfernen Sie den Datenträger jetzt</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>Sie haben keine Berechtigung, die ausgewählte Datei zu lesen.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Images können nur unkomprimiert zurückgelesen werden. Wählen Sie einen Dateinamen ohne die Endung .gz oder .xz.

Komprimierte Images (.img.gz, .img.xz) können geschrieben und geprüft werden.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Lesen fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Überprüfung fehlgeschlagen.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: mindestens %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Es kann nur der Teil verglichen werden, der darauf passt.

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich konnte nicht auf Daten geprüft werden, weil das Image komprimiert ist

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Überprüfung abgebrochen.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Überprüfen …</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Partitionstabelle beschädigt</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Reparatur fehlgeschlagen</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>Die Partitionstabelle konnte nicht repariert werden: %1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">Zu berücksichtigende Partitionen auswählen.</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Image größer als der Datenträger</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">Das Image ist größer als der Datenträger, daher konnte nur der Teil verglichen werden, der darauf passt. Alles Verglichene stimmte überein, aber der Datenträger enthält kein vollständiges Image.

Dies konnte erst am Ende des Datenträgers festgestellt werden, weil ein gzip-Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Datenträger %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Bitte geben Sie eine Image-Datei an, die Sie verwenden möchten.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Datenträger werden durchsucht …</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">Die Datei konnte zum Berechnen der Prüfsumme nicht vollständig gelesen werden:
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>Schreiben: %1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>Lesen: %1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>Überprüfen: %1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>Hashwert wird berechnet: %1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">Prüfsumme wird berechnet …</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">Prüfsumme abgebrochen.</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1 Der primäre GPT-Header verweist auf Sektoren, in denen die Partitionseinträge nicht liegen.

Genau das hinterlässt Windows, wenn es eine Karte neu einliest, die ohne „GPT nach dem Schreiben reparieren“ geschrieben wurde. Es sind keine Daten verloren gegangen, aber der Datenträger startet nicht und die meisten Programme lehnen die Tabelle ab.

Die Partitionstabelle jetzt reparieren?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Bitte wählen Sie einen Datenträger aus.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>Der Datenträger konnte nicht gesperrt werden.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>Der Datenträger konnte nicht geöffnet werden.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>Die Partitionstabelle dieses Datenträgers ist beschädigt:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Partitionstabelle repariert.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>Die Partitionstabelle ist weiterhin beschädigt.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>Die Partitionstabelle ist gültig.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Partitionstabelle</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>Die GPT auf diesem Datenträger ist gültig: Der Header und die Partitionseinträge, auf die er verweist, stimmen überein.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>Keine GPT auf diesem Datenträger.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>Dieser Datenträger hat keine GPT und kann daher den hier geprüften Schaden nicht aufweisen.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>Die Partitionstabelle konnte nicht gelesen werden.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>Die Partitionstabelle konnte nicht gelesen werden oder ist auf andere Weise beschädigt, als diese Funktion reparieren kann.</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>Der Zieldatenträger ist auch eine der Quellen.</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 liegt auf dem Zieldatenträger und kann nicht auf ihn geschrieben werden.</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>Die Geräteliste hat sich während der Bestätigung geändert. Prüfen Sie den Zieldatenträger und versuchen Sie es erneut.</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 endet bei Sektor %2, vor dem Ende der Partition, die es dort liefern soll: das Image ist unvollständig.</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>Sektor %1 des Datenträgers stimmt nicht mit Sektor %2 von %3 überein.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>Der Datenträger wurde teilweise beschrieben und enthält keine nutzbare Aufteilung mehr. Beschreiben Sie ihn erneut, bevor Sie ihn verwenden.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Schreiben …</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>Die Partitionstabelle auf dem Datenträger stimmt nicht mit dem Geschriebenen überein.</translation>
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
        <translation>Schreiben und Prüfen erfolgreich.

Der Datenträger enthält eine neue %1-Partitionstabelle mit %2 Partitionen aus %3 Images.</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Schreiben erfolgreich.

Der Datenträger enthält eine neue %1-Partitionstabelle mit %2 Partitionen aus %3 Images.</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>Ihre Sicherung liegt bereits am Ende des Datenträgers, Windows hat also nichts zu reparieren.</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>Ob er bootet, hängt davon ab, ob seine Bootloader ihre Partitionen an ihrer neuen Position finden.</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>Benutzerdefinierte Partitionierung</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>Das Image ist größer als der Datenträger, daher wurde sein Ende nicht geschrieben und der Datenträger enthält kein vollständiges Image.

Dies konnte erst festgestellt werden, als der Datenträger voll war, weil das komprimierte Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Die GPT entspricht jetzt dem Datenträger (%1), sodass Windows nichts zu reparieren hat. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image verwendet eine MBR-Partitionstabelle und keine GPT, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schreiben erfolgreich.

Dieses Image hat keine Partitionstabelle, der Windows-GPT-Fehler kann es also nicht betreffen. Der Datenträger kann normal entfernt werden.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>Der Datenträger ist offline und ausgeworfen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>Der Datenträger konnte NICHT offline geschaltet werden.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>Die GPT konnte nicht repariert werden (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>fehlerhafte GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>„GPT nach dem Schreiben reparieren“ ist deaktiviert.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Dieses Image IST betroffen: Es reserviert Platz vor der ersten Partition, sodass ein erneutes Einlesen die primäre Tabelle auf die falschen Sektoren zeigen lässt. Windows akzeptiert das Ergebnis weiterhin, Linux nicht, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Dieses Image ist NICHT betroffen: Ein erneutes Einlesen schreibt die Tabelle zwar neu, bei diesem Layout aber mit den richtigen Werten. Wird der Datenträger jetzt entfernt, bleibt er ohnehin mit dem Image identisch.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Ob dieses Image betroffen ist, konnte nicht ermittelt werden. Gehen Sie davon aus: Ein erneutes Einlesen kann eine Tabelle hinterlassen, die Linux ablehnt, und der Datenträger startet nicht.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Schreiben erfolgreich, aber die Partitionstabelle ist gefährdet.

%1 %2

%3

Entfernen Sie den Datenträger JETZT und stecken Sie ihn hier nicht wieder ein. Setzen Sie ihn direkt in die Zielhardware ein.</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>Das kombinierte Image endete vorzeitig.</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>Sektor %1 von %2 entspricht nicht dem Geschriebenen.</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 enthält mehr als das kombinierte Image oder endet nicht sauber.</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>Schreiben und Prüfen erfolgreich.</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 enthält eine %2-Partitionstabelle mit %3 Partitionen aus %4 Images.</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>Ihre Sicherungs-GPT bildet das Ende des Images; „GPT nach dem Schreiben reparieren“ verschiebt sie beim Schreiben des Images an das Ende eines größeren Datenträgers.</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">Partitionen auswählen</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">Beim Überspringen des nicht partitionierten Bereichs bleiben nur die Partitionen und die Partitionstabelle erhalten, dazu jeder Bereich, den eine GPT vor ihren Partitionen reserviert.

Manche startfähigen Images, etwa für Einplatinencomputer, speichern Bootloader-Daten außerhalb der Partitionen. Ein so gelesenes Image eines solchen Datenträgers startet womöglich nicht.</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">Beim Verkleinern bleiben nur die Partitionen und die Partitionstabelle erhalten, dazu jeder Bereich, den eine GPT vor ihren Partitionen reserviert.

Manche startfähigen Images, etwa für Einplatinencomputer, speichern Bootloader-Daten außerhalb der Partitionen. Ein verkleinertes Image eines solchen Datenträgers startet dann womöglich nicht.</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">Wählen Sie, welche Partitionen im Image enthalten sein sollen. Alles nicht Angehakte wird entfernt, genau wie nicht partitionierter Speicherplatz.</translation>
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
        <translation type="vanished">Mindestens eine Partition muss angehakt bleiben.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">Images können nur unkomprimiert zurückgelesen werden. Wählen Sie einen Dateinamen ohne die Endung .gz oder .xz, oder aktivieren Sie „Als .img.gz lesen“ oder „Als .img.xz lesen“.</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Bitte wählen Sie einen Quelldatenträger aus.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Überschreiben bestätigen</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Sind Sie sicher, dass die angegebene Datei überschrieben werden soll?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">Auf dem Datenträger wurde keine Partitionstabelle gefunden, daher gibt es nichts zur Auswahl. Der gesamte Datenträger wird gelesen.</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">Lesen abgebrochen.</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>Der Datenträger ist nicht groß genug für die angegebene Image-Datei.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Lesen …</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Lesevorgang abgebrochen.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Lesen war erfolgreich.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>Datei-Info</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Bitte geben Sie eine Datei zum Speichern der Daten an.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Fehler beim Überprüfen</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Bitte wählen Sie einen Datenträger zum Vergleichen aus.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint Daten zu ENTHALTEN

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Das Image ist größer als der Datenträger:
  Image: %1 Sektoren
  Datenträger: %2 Sektoren
  Sektorgröße: %3

Der zusätzliche Bereich scheint keine Daten zu enthalten

Trotzdem fortfahren?</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>Der Datenträger konnte bei Sektor %1 nicht gelesen werden.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>Der Datenträger enthält das Image korrekt, aber seine Partitionstabelle ist beschädigt:</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>Der Datenträger enthält das Image korrekt, aber seine Partitionstabelle ist weiterhin beschädigt. Schreiben Sie das Image erneut mit aktivierter Option „GPT nach dem Schreiben reparieren“, oder führen Sie die Überprüfung erneut aus und bestätigen Sie die Reparatur.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Überprüfung erfolgreich.

Die Partitionstabelle des Datenträgers war beschädigt und wurde repariert.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Überprüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT, und die GPT auf dem Datenträger ist gültig.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Überprüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT.</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>Größe stimmt nicht überein!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Überprüfung fehlgeschlagen</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>Überprüfung fehlgeschlagen bei Sektor: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>Das Image ist größer als der Datenträger, daher konnte nur der Teil verglichen werden, der darauf passt. Alles Verglichene stimmte überein, aber der Datenträger enthält kein vollständiges Image.

Dies konnte erst am Ende des Datenträgers festgestellt werden, weil das komprimierte Image seine unkomprimierte Größe nicht speichert.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Prüfung erfolgreich.

Image und Datenträger unterscheiden sich nur in der GPT, die von der Option „GPT nach dem Schreiben reparieren“ absichtlich neu geschrieben wird.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

Der Datenträger wurde ausgeworfen. Entfernen Sie ihn jetzt.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

Der Datenträger konnte NICHT automatisch offline geschaltet werden.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Überprüfung erfolgreich.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>Dateifehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf die Datei zu erhalten.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Datenträgerfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Fehler beim Versuch, ein Handle auf den Datenträger zu erhalten.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Der freie Speicherplatz auf dem Volume mit %1 konnte nicht ermittelt werden.
Fehler %2: %3
Die Prüfung des freien Speicherplatzes wird übersprungen.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Sperrfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">Fehler beim Versuch, den Datenträger zu sperren. Fehler %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Entsperrfehler</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger zu entsperren.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Fehler beim Aushängen</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>Fehler beim Versuch, den Datenträger auszuhängen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Lesefehler</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>Die Sektoranzahl ist zu groß.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>Für den Lesepuffer konnte kein Speicher reserviert werden.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu lesen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Fehler beim Schreiben</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Fehler beim Versuch, Daten zu schreiben.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>Der Datenträger hat nur %1 von %2 Bytes angenommen. Das Image auf dem Datenträger ist unvollständig.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>Fehler beim Versuch, die Geometrie des Datenträgers abzufragen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Fehler beim Versuch, die Dateigröße abzufragen.
Fehler %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Fehler beim freien Speicherplatz</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Fehler beim Ermitteln des freien Speicherplatzes auf Laufwerk %1. Fehler %2: %3 Überprüfung des freien Speicherplatzes wird übersprungen.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Unbekannter Datenträger</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>Die Volumes auf diesem Computer konnten nicht aufgelistet werden.
Fehler %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Volume %1 konnte nicht gesperrt werden: Es ist noch in Benutzung.
Schließen Sie alle Programme, die den Datenträger verwenden, und versuchen Sie es erneut.
Fehler %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>die Größe des primären GPT-Headers liegt außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>die Prüfsumme des primären GPT-Headers ist ungültig</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1, keine Partitionstabelle</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>unbekanntes Dateisystem, keine Partitionstabelle</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>die Größe des GPT-Headers liegt außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>die Prüfsumme des GPT-Headers ist ungültig</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>das GPT-Partitionseintragsfeld liegt nicht dort, wo der Header es angibt</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA liegt innerhalb der Partitionstabelle</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>Partition %1 reicht über das Ende des Images hinaus</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>Partition %1 beschreibt einen unmöglichen Bereich</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>die GPT enthält keine Partitionen</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>zwei Partitionen überlappen sich</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>erweitert, mit ihren logischen Partitionen (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>Typ 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>der MBR enthält keine Partitionen</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>der MBR hat mehr als eine erweiterte Partition</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>das Image ist kleiner als ein Sektor</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>das Image hat einen schützenden MBR, aber keinen GPT-Header</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>eine erweiterte MBR-Partition kann nicht in eine GPT: wählen Sie das Image der logischen Partitionen als Vorspann, oder lassen Sie sie weg</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR-Partitionstyp 0x%1 hat keine diesem Programm bekannte GPT-Entsprechung</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT-Partitionstyp %1 hat keine MBR-Entsprechung</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>es sind keine Partitionen gewählt</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>eine gewählte Partition ist nicht vorhanden</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>eine Partition ist doppelt gewählt</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>die Größe eines Images ohne Partitionstabelle ist nicht bekannt: scannen Sie es zuerst</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>das Vorspann-Image hat keine Partitionstabelle</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>ein MBR enthält höchstens vier Partitionen, gewählt sind %1</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>ein MBR kann nur eine erweiterte Partition enthalten</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>die GPT hat Platz für %1 Partitionen, gewählt sind %2</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>die Aufteilung passt nicht mehr in einen 32-Bit-MBR-Eintrag</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>die Partitionen benötigen %1 MB, der Datenträger hat %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>das GPT-Eintragsfeld passt nicht auf den Datenträger</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>die Prüfsumme des GPT-Partitionseintragsfelds ist ungültig</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>eine Partition reicht über das Ende des Datenträgers hinaus</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2; die veraltete Kopie bei LBA %3 wurde gelöscht</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>Sicherungs-GPT nach LBA %1 verschoben; letzte nutzbare LBA ist jetzt %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>das Gerät hat eine GPT, die sein MBR nur widerspiegelt</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>der MBR enthält keine Partitionen, auf die verkleinert werden könnte</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>das umgepackte Layout passt nicht mehr in einen 32-Bit-MBR-Eintrag</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>der Datenträger ist bereits so knapp bemessen; nichts zu verkleinern</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA eignet sich nicht zum Neupacken</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>die GPT enthält keine Partitionen, auf die verkleinert werden könnte</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>ein Partitionseintrag beschreibt einen unmöglichen Bereich</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>die Geometrie des Datenträgers ist unbrauchbar</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>der primäre GPT-Header ist nicht lesbar</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>die Geometrie des GPT-Eintragsfelds ist unbrauchbar</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>der Datenträger ist zu klein für ein Eintragsfeld</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>die Partitionseinträge konnten nicht gelesen werden</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>die Partitionseinträge liegen nicht bei LBA 2, daher handelt es sich nicht um den Schaden, den diese Funktion reparieren kann</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>der reparierte Header konnte nicht geschrieben werden</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA wurde wieder auf LBA 2 gesetzt und die Header-Prüfsumme neu berechnet</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>Der Datenträger meldet eine Sektorgröße von null.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>Datenträger %1 konnte nicht geöffnet werden (Fehler %2).</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>Die Größe von Datenträger %1 konnte nicht gelesen werden (Fehler %2).</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>Datenträger %1 hat %2-Byte-Sektoren, nicht %3.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>Die Image-Datei konnte nicht geöffnet werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>Die Größe der Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>Die Image-Datei konnte nicht gelesen werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>Die Image-Datei konnte nicht zurückgespult werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>Die bzip2-Dekomprimierung konnte nicht gestartet werden (bzip2-Fehler %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>Die zstd-Dekomprimierung konnte nicht gestartet werden (zstd-Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>Die gzip-Dekomprimierung konnte nicht gestartet werden (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>Die xz-Dekomprimierung konnte nicht gestartet werden (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>Die Image-Datei endet mitten in den komprimierten Daten. Sie ist abgeschnitten oder beschädigt.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>Das gzip-Image konnte nicht dekomprimiert werden.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>Das gzip-Image ist beschädigt (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>Das bzip2-Image konnte nicht dekomprimiert werden.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>Das bzip2-Image ist beschädigt (bzip2-Fehler %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>Das zstd-Image ist beschädigt (zstd-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>Das xz-Image ist beschädigt (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>Ein komprimiertes Image kann nur vorwärts gelesen werden.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>Die Image-Datei konnte nicht erstellt werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>Die gzip-Komprimierung konnte nicht gestartet werden (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>Die xz-Komprimierung konnte nicht gestartet werden (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>Die bzip2-Komprimierung konnte nicht gestartet werden (bzip2-Fehler %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>Die zstd-Komprimierung konnte nicht gestartet werden (zstd-Fehler %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>Die gzip-Komprimierung ist fehlgeschlagen (zlib-Fehler %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>Die xz-Komprimierung ist fehlgeschlagen (lzma-Fehler %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>Die bzip2-Komprimierung ist fehlgeschlagen (bzip2-Fehler %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>Die zstd-Komprimierung ist fehlgeschlagen (zstd-Fehler %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>Die Image-Datei konnte nicht geschrieben werden (Fehler %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>Die Image-Datei ist nicht zum Schreiben geöffnet.</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>Die Puffer der Image-Datei konnten nicht geleert werden (Fehler %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 endet bei Sektor %2, vor dem Ende der Partition, die es dort liefern soll: das Image ist unvollständig.</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>Datenträger %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>Quelldatenträger werden ausgehängt</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>Während sie gelesen werden, sind die Volumes auf diesen Quelldatenträgern gesperrt und ausgehängt, damit sie sich zwischendurch nicht ändern:

%1

Programme, die sie verwenden, verlieren den Zugriff bis zum Ende des Vorgangs. Auf ihnen wird nichts verändert. Fortfahren?</translation>
    </message>
</context>
</TS>
