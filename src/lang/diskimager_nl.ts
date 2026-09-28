<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="nl_NL">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>Aangepaste partitionering</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>Voeg imagebestanden of schijven toe, vink de partities aan die op het apparaat of in een nieuw imagebestand moeten komen, en zet ze op volgorde. De partitietabel van elke bron wordt uit de eerste sectoren gelezen; verder wordt niets gelezen totdat u schrijft of om een volledige scan vraagt.</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>Bronnen</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>Bron / partitie</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Grootte</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>Images toevoegen...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>Schijven toevoegen...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>Neem ook partities van schijven: geheugenkaarten, USB-sticks en andere schijven. De schijf waarvan Windows draait, wordt nooit aangeboden. Terwijl een schijf wordt gelezen, zijn de volumes erop vergrendeld en ontkoppeld.</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Verwijderen</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>Volledige scan</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>Leest en decomprimeert de hele image om de exacte grootte te bepalen en te controleren dat elke partitie tot het einde erin zit. Alleen nodig voor een image zonder partitietabel waarvan het bestand de grootte niet vastlegt, of om een gecomprimeerde image vóór het schrijven te controleren.</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Indeling</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>Partities, op volgorde:</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>Omhoog</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>Omlaag</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>Aanloop van:</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>Kopieert de opstartcode van deze image en de ruimte tussen de partitietabel en de eerste partitie (tot 32 MiB), waar een bootloader kan staan. Het apparaat krijgt dan hetzelfde soort partitietabel als deze image, en de eerste partitie begint waar die in deze image begon.</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>Op het apparaat</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Begin</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Van</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>Schrijven naar</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>Een apparaat:</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Alle apparaten tonen</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Toont ook vaste schijven. Interne PCIe-kaartlezers presenteren de kaart vaak als een niet-verwisselbaar apparaat, dat anders verborgen blijft. De schijf waarvan Windows draait, wordt nooit getoond.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>Een imagebestand:</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation type="vanished">combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Bladeren...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>Comprimeren naar</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Het gecomprimeerde formaat om naar te schrijven: .img.zst is het snelst, .img.xz het kleinst en .img.gz het breedst ondersteund</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>Controleren na schrijven</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>Schrijven...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>er is geen apparaat gekozen om naar te schrijven</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>schijf %1 kon niet worden gelezen</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>schijf %1 heeft sectoren van %2 bytes, en de bronnen van %3 bytes</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>Schijf %1: %2</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>Gecombineerde image opslaan als</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>Schijf-images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>de image eindigt binnen de eigen partitietabel</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>de partitietabel kon niet worden gelezen</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>Images toevoegen</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 kan niet worden gebruikt: %2.</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 heeft geen partitietabel en wordt daarom als één partitie behandeld: de hele image. Het bestand legt niet vast hoe groot die is, dus moet het tot het einde worden gelezen om dat te bepalen.

Nu scannen?</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>Schijven toevoegen</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>Vink de schijven aan waarvan partities worden genomen:</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>Toont ook vaste schijven. De schijf waarvan Windows draait, wordt nooit getoond.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- het apparaat waarnaar wordt geschreven</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>Al een bron.</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>Schijf %1 kan niet worden gebruikt: %2.</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>schijf</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>%1 scannen...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuleren</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>%1 kon niet tot het einde worden gelezen: %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>%1 scannen: %2 gelezen...</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 eindigt bij %2, voordat de partitie %3 eindigt: de image is onvolledig en die partitie kan niet volledig worden gekopieerd.</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>hele image</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>Partitie %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>Partitie %1: %2</translation>
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
        <translation>geen partitietabel</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>grootte niet vastgelegd</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1, gescand</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>onbekend: scan de image</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>Geen: een nieuwe, lege tabel</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>Vink de partities aan die op het apparaat moeten komen.</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>Dit kan niet worden geschreven: %1.</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>Dit kan niet worden geschreven: %1 is het apparaat waarnaar wordt geschreven. Schrijf naar een imagebestand of kies een ander apparaat.</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>Partitietabel (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>Aanloop</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>Reserve-GPT</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1, %2 partities: een imagebestand van %3.</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1, %2 partities: %3 gebruikt, %4 vrij van %5.</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>Images waarvan de grootte niet is vastgelegd, worden pas gecontroleerd bij scannen of schrijven.</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>Sommige partities delen een GUID: u krijgt daar een vraag over.</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>Geef het te schrijven imagebestand een naam.</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 is een van de images die worden gecombineerd; kies een andere naam.</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 bestaat al. Overschrijven?</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 staat op schijf %2, die een van de bronnen is: de volumes erop zijn vergrendeld terwijl die wordt gelezen, dus er kan niets naar worden geschreven. Kies een locatie op een andere schijf.</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>Dubbele partitie-GUID&apos;s</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>Deze unieke partitie-GUID&apos;s horen bij meer dan één van de gekozen partities:

%1

Meestal is het dezelfde partitie, genomen uit twee kopieën van één image. Met dubbele GUID&apos;s kan een systeem dat zijn partities via PARTUUID vindt -- in fstab of op de kernelopdrachtregel -- de verkeerde gebruiken.

Voor de latere kopieën kunnen nieuwe GUID&apos;s worden gegenereerd; de eerste behoudt de eigen GUID. Alles wat een opnieuw gegenereerde partitie bij de oude PARTUUID noemt, vindt die daarna niet meer.</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>Nieuwe GUID&apos;s genereren</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>Behouden</translation>
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
        <translation>Imagebestand</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Controleren</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Doelapparaat</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">Image verkleinen bij lezen</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">Leest de MBR of GPT van het apparaat om de image te verkleinen tot alleen de werkelijke partities. Verplaatst de reserve-GPT naar het einde van de gebruikte ruimte.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">Lezen naar .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">Comprimeert de van het apparaat gelezen image met gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">Lezen naar .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">Comprimeert de van het apparaat gelezen image met xz</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">Te lezen partities kiezen</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">Toont voor het lezen de partities van het apparaat en laat kiezen welke worden opgenomen. Alles wat wordt weggelaten, wordt uit de image verwijderd, net als niet-gepartitioneerde ruimte -- dit verkleint de image altijd, ongeacht of &quot;Image verkleinen bij lezen&quot; ook is aangevinkt.</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>Sluit WinDiskImager af</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Sluit Win Disk Imager af</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">GPT controleren</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Controleer het geselecteerde apparaat op een beschadigde GPT en bied aan deze te herstellen.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">Niet-gepartitioneerde ruimte overslaan</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Leest de MBR of GPT van het apparaat en laat de niet-gepartitioneerde ruimte weg uit de image. De partities, de partitietabel en de ruimte die een GPT vóór zijn partities reserveert blijven behouden. De reserve-GPT wordt naar het nieuwe einde van de image verplaatst.</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>Comprimeren tijdens lezen</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>Comprimeert de van het apparaat gelezen image in het hieronder gekozen formaat</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">Het gecomprimeerde formaat om naar te lezen: .img.gz is sneller gemaakt, .img.xz is kleiner</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Leest de MBR of GPT van het apparaat en laat de niet-gepartitioneerde ruimte tussen en na de partities weg. De ruimte vóór de eerste partitie, waar een bootloader staat, wordt tot 32 MB na de partitietabel ongewijzigd gelezen; alleen wat daarna komt wordt weggelaten. De reserve-GPT wordt naar het nieuwe einde van de image verplaatst.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>Leest de MBR of GPT van het apparaat en laat de niet-gepartitioneerde ruimte tussen en na de partities weg. Tot 32 MB van de ruimte vóór de eerste partitie, waar een bootloader in ongebruikte ruimte kan staan, wordt ongewijzigd gelezen; alleen wat daarna komt wordt weggelaten. De reserve-GPT wordt naar het nieuwe einde van de image verplaatst.</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">Toont voor het lezen de partities van het apparaat en laat kiezen welke worden opgenomen. Alles wat wordt weggelaten, wordt uit de image verwijderd, net als niet-gepartitioneerde ruimte -- niet-gepartitioneerde ruimte wordt daarbij altijd ook overgeslagen, ongeacht of &quot;Niet-gepartitioneerde ruimte overslaan&quot; is aangevinkt.</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>Te lezen partities kiezen</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>Slechts enkele partities van het apparaat lezen: opent Aangepaste partitionering met het apparaat als bron, alle partities aangevinkt en het imagebestand als bestemming. Vink uit wat moet worden weggelaten.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>Niet-gepartitioneerde ruimte overslaan bij lezen</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Hulpmiddelen</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>GPT van apparaat controleren</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>Aangepaste partitionering...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>Partities uit imagebestanden en van schijven op een apparaat of in een nieuw imagebestand zetten, in een volgorde naar keuze, onder een nieuwe partitietabel.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Hash van imagebestand</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Hashtype dat voor het imagebestand gegenereerd wordt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Gekozen hashtype voor het bestand genereren</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Genereer</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Kopieer hash naar klembord</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopieer</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>GPT herstellen na schrijven</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Verplaats na het schrijven de reserve-GPT naar het einde van het apparaat en werk de header bij zodat Windows niets te &quot;repareren&quot; heeft. Laat dit uit om in plaats daarvan gewaarschuwd te worden het apparaat te verwijderen.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Alle apparaten tonen</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Toon ook vaste schijven. Interne PCIe-kaartlezers presenteren de kaart vaak als een niet-verwisselbaar apparaat, dat anders verborgen blijft. De schijf waarvan Windows draait wordt nooit getoond.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Leest de MBR of GPT van het apparaat en laat de niet-gepartitioneerde ruimte tussen en na de partities weg. Alles vóór de eerste partitie, waar een bootloader staat, wordt ongewijzigd gelezen, en de eerste partitie wordt niet verplaatst. De reserve-GPT wordt naar het nieuwe einde van de image verplaatst.</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Het gecomprimeerde formaat om naar te lezen: .img.zst is het snelst, .img.xz het kleinst en .img.gz het breedst ondersteund</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Voortgang</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Breek huidig proces af.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuleren</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Gegevens van het apparaat lezen naar het imagebestand</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Lezen</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Gegevens van het imagebestand naar het apparaat schrijven</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Schrijven</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Gegevens op het apparaat vergelijken met het imagebestand</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Controleer het image met de geselecteerde schijf</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Alleen controleren</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Sluit Win32 Disk Imager af</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Afsluiten</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>Afsluiten?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Nu afsluiten resulteert in een corrupt imagebestand.
Weet u zeker dat u wilt afsluiten?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Nu afsluiten resulteert in een corrupte schijf.
Weet u zeker dat u wilt afsluiten?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Kies een imagebestand</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Bezig met genereren…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>Afbreken?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Nu afbreken resulteert in een corrupt doel.
Weet u zeker dat u wilt afbreken?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Schrijffout</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>Het imagebestand mag zich niet op het doelapparaat bevinden.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Overschrijven bevestigen</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Wacht op een taak.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Nu afsluiten breekt het controleren van het image af.
Weet u zeker dat u wilt afsluiten?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Controleren afbreken.
Weet u zeker dat u wilt afbreken?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>Niet genoeg beschikbare ruimte!</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>Bestandsfout</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Schrijven geslaagd, maar de partitietabel loopt gevaar.

%1
%2

%3

Verwijder het apparaat NU fysiek, voordat u iets anders doet, en plaats het niet opnieuw in deze computer. Plaats het in plaats daarvan in de doelhardware.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>Het gekozen bestand bestaat niet.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>Het gekozen bestand bevat geen data.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Voltooid.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Afgerond</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Schrijven is gelukt.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>Schijf-images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>Gecomprimeerde schijf-images (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fout</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">Kon het bestand niet openen om een controlesom te genereren:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Selecteer een doelapparaat.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Alle bestanden en gegevens op dit apparaat worden verwijderd.
(Doelapparaat: %1)
Weet u zeker dat u wilt doorgaan?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>Apparaat heeft aangekoppelde volumes</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 is in Windows aangekoppeld als %2.

Alles op dit apparaat, op elke partitie ervan, wordt vernietigd en kan niet worden hersteld.

Controleer of %2 geen station is dat u wilde behouden.

Toch naar dit apparaat schrijven?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Schrijven mislukt.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Apparaatfout</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>Het apparaat meldt een grootte van nul. Als het een kaartlezer is, is de kaart mogelijk verwijderd.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>Het bestand kon niet worden geopend om een hash te berekenen:
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>Bezig met hash berekenen…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>Het bestand kon niet volledig worden gelezen om een hash te berekenen:
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>Hash berekenen geannuleerd.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>De image is groter dan het apparaat:
  Image: minstens %1 sectoren
  Beschikbaar: %2 sectoren
  Sectorgrootte: %3

Het einde van de image wordt niet geschreven, dus het apparaat bevat geen volledige image.

Toch doorgaan?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Er is meer ruimte nodig dan beschikbaar is:
  Nodig: %1 sectoren
  Beschikbaar: %2 sectoren
  Sectorgrootte: %3

De extra ruimte kon niet op gegevens worden gecontroleerd, omdat de image gecomprimeerd is

Toch doorgaan?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Er is meer ruimte nodig dan beschikbaar is:
  Nodig: %1 sectoren
  Beschikbaar: %2 sectoren
  Sectorgrootte: %3

De extra ruimte lijkt WEL gegevens te bevatten

Toch doorgaan?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Er is meer ruimte nodig dan beschikbaar is:
  Nodig: %1 sectoren
  Beschikbaar: %2 sectoren
  Sectorgrootte: %3

De extra ruimte lijkt geen gegevens te bevatten

Toch doorgaan?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Schrijven geannuleerd.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Oude partitietabellen wissen…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Kon de bestaande partitietabellen op het apparaat niet wissen.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>Het apparaat is gedeeltelijk beschreven en bevat geen bruikbare image meer. Schrijf de image opnieuw voordat u het apparaat gebruikt.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>GPT herstellen…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Image afgekapt</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Schrijven geslaagd.

De GPT is consistent gemaakt met het apparaat (%1), dus Windows heeft geen beschadigde tabel om te repareren. Het apparaat kan normaal worden verwijderd.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Schrijven geslaagd.

De image bevat geen GPT, dus er is geen partitietabel die Windows kan repareren. Het apparaat kan normaal worden verwijderd.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Schrijven geslaagd.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Schrijven geslaagd</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">Het apparaat is offline gehaald en uitgeworpen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">Het apparaat kon NIET automatisch offline worden gehaald.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">De GPT kon niet automatisch worden hersteld (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">de GPT is misvormd</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>Het herstellen van de GPT is mislukt (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>schrijffout</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">De optie &quot;GPT herstellen na schrijven&quot; is niet ingeschakeld.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">Deze image WORDT getroffen door de Windows GPT-herschrijffout.

Er wordt ruimte gereserveerd vóór de eerste partitie, waardoor Windows bij een herscan de primaire partitietabel herschrijft en die naar de verkeerde sectoren laat wijzen. Het resultaat doorstaat de eigen controles van Windows nog steeds, maar Linux wijst het af en het apparaat start niet op.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">Deze image wordt NIET getroffen door de Windows GPT-herschrijffout.

Windows herschrijft de tabel bij een herscan nog steeds, omdat de reserve-GPT niet aan het einde van het apparaat staat, maar bij deze indeling komt de herschrijving op de juiste waarden uit. Het apparaat nu verwijderen houdt het hoe dan ook byte-identiek aan de image.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">Of deze image getroffen wordt door de Windows GPT-herschrijffout kon niet worden vastgesteld. Ga ervan uit dat dit zo is: een herscan kan een partitietabel achterlaten die Linux afwijst, waarmee het apparaat niet opstart.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Verwijder het apparaat nu</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>U heeft geen rechten om het geselecteerde bestand te lezen.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Images kunnen alleen ongecomprimeerd worden teruggelezen. Kies een bestandsnaam zonder .gz- of .xz-extensie.

Gecomprimeerde images (.img.gz, .img.xz) kunnen wel worden geschreven en geverifieerd.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Lezen mislukt.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Verifiëren mislukt.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>De image is groter dan het apparaat:
  Image: minstens %1 sectoren
  Apparaat: %2 sectoren
  Sectorgrootte: %3

Alleen het deel dat past kan worden vergeleken.

Toch doorgaan?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Image groter dan apparaat:
  Image: %1 sectoren
  Apparaat: %2 sectoren
  Sectorgrootte: %3

De extra ruimte kon niet op gegevens worden gecontroleerd, omdat de image gecomprimeerd is

Toch doorgaan?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Verifiëren geannuleerd.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Bezig met verifiëren…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Partitietabel beschadigd</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Herstellen mislukt</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>De partitietabel kon niet worden hersteld: %1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">Selecteer de partities die in de image moeten worden opgenomen.</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>Het apparaat kon niet worden gelezen op sector %1.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>Het apparaat bevat de image correct, maar de partitietabel is beschadigd:</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Image groter dan apparaat</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>Het apparaat bevat de image correct, maar de partitietabel is nog steeds beschadigd. Schrijf de image opnieuw met &quot;GPT herstellen na schrijven&quot; aangevinkt, of voer de controle opnieuw uit en accepteer het herstel.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Controle geslaagd.

De partitietabel van het apparaat was beschadigd en is hersteld.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Controle geslaagd.

De image en het apparaat verschillen alleen in de GPT, en de GPT op het apparaat is geldig.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Controle geslaagd.

De image en het apparaat verschillen alleen in de GPT.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Schijf %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Geef een imagebestand op om te gebruiken.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Bezig met zoeken naar schijven…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">Kon het hele bestand niet lezen om een controlesom te genereren:
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>Bezig met schrijven: %1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>Bezig met lezen: %1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>Bezig met verifiëren: %1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>Hash berekenen: %1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">Bezig met controlesom genereren…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">Controlesom geannuleerd.</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1: de primaire GPT-header wijst naar sectoren waar de partitie-items niet staan.

Dit is wat Windows achterlaat wanneer het een kaart opnieuw scant die zonder &quot;GPT herstellen na schrijven&quot; is geschreven. Er zijn geen gegevens verloren gegaan, maar het apparaat start niet op en de meeste hulpprogramma&apos;s weigeren de tabel.

De partitietabel nu herstellen?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Selecteer een apparaat.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>Kon het apparaat niet vergrendelen.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>Kon het apparaat niet openen.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>De partitietabel van dit apparaat is beschadigd:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Partitietabel hersteld.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>Partitietabel is nog steeds beschadigd.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>Partitietabel is geldig.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Partitietabel</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>De GPT op dit apparaat is geldig: de header en de partitie-items waarnaar deze wijst komen overeen.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>Geen GPT op dit apparaat.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>Dit apparaat heeft geen GPT, dus de schade waarop hier wordt gecontroleerd kan zich niet voordoen.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>Kon de partitietabel niet lezen.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>De partitietabel kon niet worden gelezen, of is op een andere manier beschadigd dan deze functie herstelt.</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>Het doelapparaat is ook een van de bronnen.</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 staat op het doelapparaat en kan er niet naar worden geschreven.</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>De apparatenlijst is gewijzigd tijdens het bevestigen. Controleer het doelapparaat en probeer het opnieuw.</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 eindigt bij sector %2, voordat de partitie die het daar moet leveren eindigt: de image is onvolledig.</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>Sector %1 van het apparaat komt niet overeen met sector %2 van %3.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>Het apparaat is gedeeltelijk beschreven en bevat geen bruikbare indeling meer. Schrijf het opnieuw voordat u het gebruikt.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Bezig met schrijven…</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>De partitietabel op het apparaat komt niet overeen met wat is geschreven.</translation>
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
        <translation>Schrijven en controleren geslaagd.

Het apparaat bevat een nieuwe %1-partitietabel met %2 partities uit %3 images.</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Schrijven geslaagd.

Het apparaat bevat een nieuwe %1-partitietabel met %2 partities uit %3 images.</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>De reservekopie staat al aan het einde van het apparaat, dus Windows hoeft niets te herstellen.</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>Of het opstart, hangt ervan af of de bootloaders hun partities vinden op de plek waar ze nu staan.</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>Aangepaste partitionering</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>De image is groter dan het apparaat, dus het einde ervan is niet geschreven en het apparaat bevat geen volledige image.

Dit kon pas worden vastgesteld toen het apparaat vol was, omdat de gecomprimeerde image zijn ongecomprimeerde grootte niet vastlegt.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Schrijven geslaagd.

De GPT komt nu overeen met het apparaat (%1), dus Windows heeft niets te repareren. Verwijder het apparaat normaal.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schrijven geslaagd.

Deze image gebruikt een MBR-partitietabel, geen GPT, dus de GPT-herschrijffout van Windows kan er geen invloed op hebben. Verwijder het apparaat normaal.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Schrijven geslaagd.

Deze image heeft geen partitietabel, dus de GPT-herschrijffout van Windows kan er geen invloed op hebben. Verwijder het apparaat normaal.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>Het apparaat is offline en uitgeworpen.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>Het apparaat kon NIET offline worden gezet.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>De GPT kon niet worden hersteld (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>onjuiste GPT</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>&quot;GPT herstellen na schrijven&quot; staat uit.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Deze image IS getroffen: er wordt ruimte vóór de eerste partitie gereserveerd, waardoor een herscan de primaire tabel naar de verkeerde sectoren laat wijzen. Windows accepteert het resultaat nog wel, Linux niet, en het apparaat start niet op.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Deze image is NIET getroffen: een herscan herschrijft de tabel nog steeds, maar bij deze indeling met de juiste waarden. Het apparaat nu verwijderen houdt het hoe dan ook identiek aan de image.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Of deze image getroffen is, kon niet worden vastgesteld. Ga ervan uit dat dit zo is: een herscan kan een tabel achterlaten die Linux weigert, waarna het apparaat niet opstart.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Schrijven geslaagd, maar de partitietabel loopt risico.

%1 %2

%3

Verwijder het apparaat NU en plaats het hier niet opnieuw. Steek het meteen in de doelhardware.</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>De gecombineerde image eindigde te vroeg.</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>Sector %1 van %2 is niet wat er is geschreven.</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 bevat meer dan de gecombineerde image, of eindigt niet netjes.</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>Schrijven en controleren geslaagd.</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 bevat een %2-partitietabel met %3 partities uit %4 images.</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>De reserve-GPT staat aan het einde van de image; &quot;GPT herstellen na schrijven&quot; verplaatst die naar het einde van een groter apparaat wanneer de image wordt geschreven.</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">Partities kiezen</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">Bij het overslaan van niet-gepartitioneerde ruimte blijven alleen de partities en de partitietabel over, plus de ruimte die een GPT vóór zijn partities reserveert.

Sommige opstartbare images, zoals die voor single-board computers, bewaren bootloadergegevens buiten de partities. Een op deze manier gelezen image van zo&apos;n apparaat start mogelijk niet op.</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">Bij verkleinen blijven alleen de partities en de partitietabel over, plus de ruimte die een GPT vóór zijn partities reserveert.

Sommige opstartbare images, zoals die voor single-board computers, bewaren bootloadergegevens buiten de partities. Een verkleinde image van zo&apos;n apparaat start mogelijk niet op.</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">Kies welke partities in de image worden opgenomen. Alles wat niet is aangevinkt, wordt verwijderd, net als niet-gepartitioneerde ruimte.</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">Partitie %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">Partitie %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">Er moet minstens één partitie aangevinkt blijven.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Leesfout</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">Images kunnen alleen ongecomprimeerd worden teruggelezen. Kies een bestandsnaam zonder .gz- of .xz-extensie, of vink &quot;Lezen naar .img.gz&quot; of &quot;Lezen naar .img.xz&quot; aan.</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Selecteer een bronapparaat.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Bevestig overschrijven</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Weet u zeker dat u dit bestand wilt overschrijven?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">Er is geen partitietabel op het apparaat gevonden, dus er is niets om te kiezen. Het hele apparaat wordt gelezen.</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">Lezen geannuleerd.</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>De schijf is niet groot genoeg voor het opgegeven imagebestand.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Bezig met lezen…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Lezen is afgebroken.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Lezen is gelukt.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>Bestandsinformatie</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Geef een bestand op om de gegevens in op te slaan.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Controlefout</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Selecteer een apparaat om mee te vergelijken.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Image groter dan apparaat:
  Image: %1 sectoren
  Apparaat: %2 sectoren
  Sectorgrootte: %3

De extra ruimte lijkt WEL gegevens te bevatten

Toch doorgaan?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Image groter dan apparaat:
  Image: %1 sectoren
  Apparaat: %2 sectoren
  Sectorgrootte: %3

De extra ruimte lijkt geen gegevens te bevatten

Toch doorgaan?</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>Grootte komt niet overeen!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Controle mislukt</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>Controle mislukt op sector %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>De image is groter dan het apparaat, dus alleen het deel dat past kon worden vergeleken. Alles wat is vergeleken kwam overeen, maar het apparaat bevat geen volledige image.

Dit kon pas aan het einde van het apparaat worden vastgesteld, omdat de gecomprimeerde image zijn ongecomprimeerde grootte niet vastlegt.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Verificatie geslaagd.

De image en het apparaat verschillen alleen in de GPT, die de optie &quot;GPT herstellen na schrijven&quot; met opzet herschrijft.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

Het apparaat is uitgeworpen. Verwijder het nu.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

Het apparaat kon NIET automatisch offline worden gehaald.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Controle geslaagd.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>Bestandsfout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het opvragen van de handle van het bestand.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Apparaatfout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het opvragen van de handle van het apparaat.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Fout bij het opvragen van de vrije ruimte op het volume dat %1 bevat.
Fout %2: %3
Controle van de vrije ruimte wordt overgeslagen.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Vergrendelingsfout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translatorcomment>wat is de nederlandse vertaling van volume?</translatorcomment>
        <translation type="vanished">Een fout is opgetreden bij het vergrendelen van het volume
Error %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Ontgrendelingsfout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translatorcomment>wat is de nederlandse vertaling van volume?</translatorcomment>
        <translation>Een fout is opgetreden bij het ontgrendelen van het volume.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Ontkoppelfout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translatorcomment>wat is de nederlandse vertaling van volume?</translatorcomment>
        <translation>Een fout is opgetreden bij het ontkoppelen van het volume.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Leesfout</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>Het aantal sectoren is te groot.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>Kan geen geheugen reserveren voor de leesbuffer.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het lezen van data van de handle.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Schrijffout</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het schrijven van data naar de handle.
Fout %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>Het apparaat heeft slechts %1 van %2 bytes geaccepteerd. De image op het apparaat is onvolledig.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het opvragen van de geometrie van het apparaat.
Fout %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Een fout is opgetreden bij het opvragen van de bestandsgrootte.
Fout %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Fout bij beschikbare ruimte</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Fout bij het opvragen van de vrije ruimte op de drive %1.
Fout %2: %3
Controle van vrije ruimte zal worden overgeslagen.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Onbekend apparaat</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>De volumes op deze computer konden niet worden opgesomd.
Fout %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Kon volume %1 niet vergrendelen: het is nog in gebruik.
Sluit elk programma dat het apparaat gebruikt en probeer het opnieuw.
Fout %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>de grootte van de primaire GPT-header ligt buiten het bereik</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>de controlesom van de primaire GPT-header is ongeldig</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1, geen partitietabel</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>onbekend bestandssysteem, geen partitietabel</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>de grootte van de GPT-header valt buiten het geldige bereik</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>de controlesom van de GPT-header is ongeldig</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>de GPT-partitietabel staat niet waar de header aangeeft</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA ligt binnen de partitietabel</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>partitie %1 loopt voorbij het einde van de image</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>partitie %1 beschrijft een onmogelijk bereik</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>de GPT bevat geen partities</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>twee partities overlappen</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>uitgebreid, met de logische partities (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>type 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>de MBR bevat geen partities</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>de MBR heeft meer dan één uitgebreide partitie</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>de image is kleiner dan één sector</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>de image heeft een beschermende MBR maar geen GPT-header</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>een uitgebreide MBR-partitie kan niet op een GPT: kies de image van de logische partities als aanloop, of laat die weg</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>MBR-partitietype 0x%1 heeft geen GPT-equivalent dat dit programma kent</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>GPT-partitietype %1 heeft geen MBR-equivalent</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>er zijn geen partities gekozen</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>een gekozen partitie bestaat niet</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>een partitie is twee keer gekozen</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>de grootte van een image zonder partitietabel is niet bekend: scan die eerst</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>de aanloop-image heeft geen partitietabel</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>een MBR bevat hoogstens vier partities, en er zijn er %1 gekozen</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>een MBR kan maar één uitgebreide partitie bevatten</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>de GPT heeft ruimte voor %1 partities, en er zijn er %2 gekozen</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>de indeling past niet meer in een 32-bits MBR-item</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>de partities hebben %1 MB nodig en het apparaat heeft %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>de GPT-itemtabel past niet op het apparaat</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>de controlesom van de GPT-partitietabel is ongeldig</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>een partitie loopt door tot voorbij het einde van het apparaat</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>reserve-GPT verplaatst naar LBA %1; laatste bruikbare LBA is nu %2; de verouderde kopie op LBA %3 is gewist</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>reserve-GPT verplaatst naar LBA %1; laatste bruikbare LBA is nu %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>het apparaat heeft een GPT, die de MBR alleen weerspiegelt</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>de MBR bevat geen partities om naar te verkleinen</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>de herverpakte indeling past niet meer in een 32-bits MBR-item</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>het apparaat is al zo krap; niets te verkleinen</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA is niet bruikbaar om opnieuw in te delen</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>de GPT bevat geen partities om naar te verkleinen</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>een partitie-item beschrijft een onmogelijk bereik</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>de geometrie van het apparaat is onbruikbaar</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>de primaire GPT-header is niet leesbaar</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>de geometrie van de GPT-itemtabel is onbruikbaar</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>het apparaat is te klein voor een itemtabel</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>de partitie-items konden niet worden gelezen</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>de partitie-items staan niet op LBA 2, dus dit is niet de schade die hiermee kan worden hersteld</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>de herstelde header kon niet worden weggeschreven</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA wijst weer naar LBA 2 en de controlesom van de header is opnieuw berekend</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>Het apparaat meldt een sectorgrootte van nul.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>Schijf %1 kon niet worden geopend (fout %2).</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>De grootte van schijf %1 kon niet worden gelezen (fout %2).</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>Schijf %1 heeft sectoren van %2 bytes, niet %3.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>Het imagebestand kon niet worden geopend (fout %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>De grootte van het imagebestand kon niet worden gelezen (fout %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>Het imagebestand kon niet worden gelezen (fout %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>Het imagebestand kon niet worden teruggespoeld (fout %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>De bzip2-decompressor kon niet worden gestart (bzip2-fout %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>De zstd-decompressor kon niet worden gestart (zstd-fout %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>De gzip-decompressor kon niet worden gestart (zlib-fout %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>De xz-decompressor kon niet worden gestart (lzma-fout %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>Het imagebestand eindigt midden in de gecomprimeerde gegevens. Het is afgekapt of beschadigd.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>De gzip-image kon niet worden gedecomprimeerd.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>De gzip-image is beschadigd (zlib-fout %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>De bzip2-image kon niet worden gedecomprimeerd.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>De bzip2-image is beschadigd (bzip2-fout %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>De zstd-image is beschadigd (zstd-fout %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>De xz-image is beschadigd (lzma-fout %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>Een gecomprimeerde image kan alleen voorwaarts worden gelezen.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>Het imagebestand kon niet worden aangemaakt (fout %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>De gzip-compressor kon niet worden gestart (zlib-fout %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>De xz-compressor kon niet worden gestart (lzma-fout %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>De bzip2-compressor kon niet worden gestart (bzip2-fout %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>De zstd-compressor kon niet worden gestart (zstd-fout %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>De gzip-compressor is mislukt (zlib-fout %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>De xz-compressor is mislukt (lzma-fout %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>De bzip2-compressor is mislukt (bzip2-fout %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>De zstd-compressor is mislukt (zstd-fout %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>Het imagebestand kon niet worden geschreven (fout %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>Het imagebestand is niet geopend om te schrijven.</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>Het imagebestand kon niet worden doorgespoeld (fout %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 eindigt bij sector %2, voordat de partitie die het daar moet leveren eindigt: de image is onvolledig.</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>Schijf %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>Bronschijven worden ontkoppeld</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>Terwijl ze worden gelezen, zijn de volumes op deze bronschijven vergrendeld en ontkoppeld, zodat niets ze halverwege verandert:

%1

Programma&apos;s die ze gebruiken, verliezen ze tot het einde van de bewerking. Er wordt niets op ze gewijzigd. Doorgaan?</translation>
    </message>
</context>
</TS>
