<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>Partitionnement personnalisé</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>Ajoutez des fichiers image ou des disques, cochez les partitions à placer sur le périphérique ou dans un nouveau fichier image, puis ordonnez-les. La table de partitions de chaque source est lue dans ses premiers secteurs ; rien d&apos;autre n&apos;est lu avant l&apos;écriture ou une analyse complète.</translation>
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
        <translation>Taille</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>Ajouter des images...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>Ajouter des disques...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>Prendre aussi des partitions sur des disques : cartes, clés USB et autres disques. Le disque depuis lequel Windows s&apos;exécute n&apos;est jamais proposé. Pendant la lecture d&apos;un disque, ses volumes sont verrouillés et démontés.</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Retirer</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>Analyse complète</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>Lit et décompresse l&apos;image entière pour connaître sa taille exacte et vérifier qu&apos;elle contient chaque partition jusqu&apos;à sa fin. Nécessaire uniquement pour une image sans table de partitions dont le fichier n&apos;indique pas la taille, ou pour vérifier une image compressée avant de l&apos;écrire.</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>Partitions, dans l&apos;ordre :</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>Monter</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>Descendre</translation>
    </message>
    <message>
        <source>Free space:</source>
        <translation>Espace libre :</translation>
    </message>
    <message>
        <source> MiB</source>
        <translation> MiB</translation>
    </message>
    <message>
        <source>How much unpartitioned space to insert, or, with free space selected in the order, how much it is. The partition after it still starts on a 1 MiB boundary.</source>
        <translation>La quantité d&apos;espace non partitionné à insérer, ou, si un espace libre est sélectionné dans l&apos;ordre, sa taille. La partition qui le suit commence toujours sur une limite de 1 MiB.</translation>
    </message>
    <message>
        <source>Insert</source>
        <translation>Insérer</translation>
    </message>
    <message>
        <source>Leave this much unpartitioned space after the selected item of the order, or at the end.</source>
        <translation>Laisser cette quantité d&apos;espace non partitionné après l&apos;élément sélectionné de l&apos;ordre, ou à la fin.</translation>
    </message>
    <message>
        <source>Remove free space</source>
        <translation>Retirer l&apos;espace libre</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>Zone de démarrage de :</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>Copie le code d&apos;amorçage de cette image, ainsi que l&apos;espace entre sa table de partitions et sa première partition (jusqu&apos;à 32 MiB), où peut se trouver un chargeur d&apos;amorçage. Le périphérique reçoit alors le même type de table de partitions que cette image, et la première partition commence là où commençait celle de cette image.</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>Sur le périphérique</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Provenance</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>Écrire vers</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>Un périphérique :</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Afficher tous les périphériques</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Affiche aussi les disques fixes. Les lecteurs de cartes PCIe internes présentent souvent la carte comme un périphérique non amovible, qui est sinon masqué. Le disque depuis lequel Windows s&apos;exécute n&apos;est jamais affiché.</translation>
    </message>
    <message>
        <source>Keep the device&apos;s partitions</source>
        <translation>Conserver les partitions du périphérique</translation>
    </message>
    <message>
        <source>Add to what the device holds instead of replacing it: its partitions stay where they are, untouched, and the new ones go into its free space, where the order puts them. Untick one of them in the order to take it out of the table; its space is then free. The table keeps its kind, and its backup GPT is moved to the end of the device.</source>
        <translation>Ajoute à ce que contient le périphérique au lieu de le remplacer : ses partitions restent là où elles sont, intactes, et les nouvelles vont dans son espace libre, là où l&apos;ordre les place. Décochez-en une dans l&apos;ordre pour la retirer de la table ; son espace devient alors libre. La table conserve son type, et sa GPT de secours est déplacée à la fin du périphérique.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>Un fichier image :</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation type="vanished">combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>Compresser en</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Le format compressé d&apos;écriture : .img.zst est le plus rapide, .img.xz le plus petit et .img.gz le plus largement pris en charge</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>Vérifier après l&apos;écriture</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>Écrire...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>aucun périphérique n&apos;est choisi comme destination</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>le disque %1 n&apos;a pas pu être lu</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>le disque %1 a des secteurs de %2 octets, et les sources des secteurs de %3 octets</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>Disque %1 : %2</translation>
    </message>
    <message>
        <source>disk %1 could not be read: %2</source>
        <translation>le disque %1 n&apos;a pas pu être lu : %2</translation>
    </message>
    <message>
        <source>disk %1 has no partition table to keep</source>
        <translation>le disque %1 n&apos;a aucune table de partitions à conserver</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>Enregistrer l&apos;image combinée sous</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>Images disque (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>l&apos;image se termine à l&apos;intérieur de sa propre table de partitions</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>la table de partitions n&apos;a pas pu être lue</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>Ajouter des images</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 ne peut pas être utilisé : %2.</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 n&apos;a pas de table de partitions ; il est donc considéré comme une seule partition : l&apos;image entière. Le fichier n&apos;indique pas sa taille, il faut donc le lire jusqu&apos;au bout pour la connaître.

L&apos;analyser maintenant ?</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>Ajouter des disques</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>Cochez les disques sur lesquels prendre des partitions :</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>Affiche aussi les disques fixes. Le disque depuis lequel Windows s&apos;exécute n&apos;est jamais affiché.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- le périphérique en cours d&apos;écriture</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>Déjà une source.</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>Le disque %1 ne peut pas être utilisé : %2.</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>disque</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>Analyse de %1…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>%1 n&apos;a pas pu être lu jusqu&apos;au bout : %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>Analyse de %1 : %2 lus…</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 se termine à %2, avant la fin de sa partition %3 : l&apos;image est incomplète et cette partition ne peut pas être copiée entièrement.</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>image entière</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>Partition %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>Partition %1 : %2</translation>
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
        <translation>aucune table de partitions</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>taille non indiquée</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1, analysée</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>inconnue : analysez l&apos;image</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>Aucune : une nouvelle table vide</translation>
    </message>
    <message>
        <source>Free space: %1 MiB</source>
        <translation>Espace libre : %1 MiB</translation>
    </message>
    <message>
        <source>This device, %1</source>
        <translation>Ce périphérique, %1</translation>
    </message>
    <message>
        <source>%1 -- taken out of the table</source>
        <translation>%1 -- retirée de la table</translation>
    </message>
    <message>
        <source>%1 -- kept</source>
        <translation>%1 -- conservée</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>Cochez les partitions à placer sur le périphérique.</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>Écriture impossible : %1.</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>Écriture impossible : %1 est le périphérique en cours d&apos;écriture. Écrivez vers un fichier image ou choisissez un autre périphérique.</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>Table de partitions (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>Zone de démarrage</translation>
    </message>
    <message>
        <source>Free space</source>
        <translation>Espace libre</translation>
    </message>
    <message>
        <source>%1, kept</source>
        <translation>%1, conservée</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>GPT de secours</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1, %2 partitions : un fichier image de %3.</translation>
    </message>
    <message>
        <source>%1, %2 partitions, %3 of them kept: %4 used, %5 free of %6.</source>
        <translation>%1, %2 partitions, dont %3 conservées : %4 utilisés, %5 libres sur %6.</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1, %2 partitions : %3 utilisés, %4 libres sur %5.</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>Les images de taille non indiquée ne sont vérifiées qu&apos;à l&apos;analyse ou à l&apos;écriture.</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>Certaines partitions partagent un GUID : la question vous sera posée.</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>Nommez le fichier image à écrire.</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 est l&apos;une des images combinées ; choisissez un autre nom.</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 existe déjà. L&apos;écraser ?</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 se trouve sur le disque %2, qui est l&apos;une des sources : ses volumes sont verrouillés pendant sa lecture, rien ne peut donc y être écrit. Choisissez un emplacement sur un autre disque.</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>GUID de partition en double</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>Ces GUID de partition uniques appartiennent à plusieurs des partitions choisies :

%1

Il s&apos;agit généralement de la même partition prise dans deux copies d&apos;une même image. Avec des GUID en double, un système qui trouve ses partitions par PARTUUID -- dans fstab ou sur la ligne de commande du noyau -- risque d&apos;utiliser la mauvaise.

De nouveaux GUID peuvent être générés pour les copies suivantes ; la première garde le sien. Tout ce qui désigne une partition régénérée par son ancien PARTUUID ne la trouvera alors plus.</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>Générer de nouveaux GUID</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>Les conserver</translation>
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
        <translation>Fichier image</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Vérifier</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">Réduire l&apos;image à la lecture</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">Lit le MBR ou la GPT du périphérique pour réduire l&apos;image aux partitions réelles uniquement. Déplace la GPT de secours à la fin de l&apos;espace utilisé.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">Lire vers .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">Compresse l&apos;image lue depuis le périphérique avec gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">Lire vers .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">Compresse l&apos;image lue depuis le périphérique avec xz</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">Choisir les partitions à lire</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">Avant la lecture, affiche les partitions du périphérique et permet de choisir lesquelles inclure. Tout ce qui est laissé de côté est retiré de l&apos;image, comme l&apos;espace non partitionné -- cela réduit toujours l&apos;image, que « Réduire l&apos;image à la lecture » soit également cochée ou non.</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>Fermer WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Fermer Win Disk Imager</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">Vérifier la GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Vérifie si la GPT du périphérique sélectionné est endommagée et propose de la réparer.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">Ignorer l&apos;espace non partitionné</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lit le MBR ou la GPT du périphérique et laisse l&apos;espace non partitionné hors de l&apos;image, en conservant les partitions, la table de partitions et l&apos;espace qu&apos;une GPT réserve avant ses partitions. La GPT de secours est déplacée à la nouvelle fin de l&apos;image.</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>Compresser pendant la lecture</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>Compresse l&apos;image lue depuis le périphérique dans le format choisi ci-dessous</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">Le format compressé de lecture : .img.gz est plus rapide à créer, .img.xz est plus petit</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lit le MBR ou la GPT du périphérique et laisse de côté l&apos;espace non partitionné entre ses partitions et après elles. L&apos;espace qui précède la première partition, où se trouve un chargeur d&apos;amorçage, est lu tel quel jusqu&apos;à 32 Mo après la table de partitions ; seul ce qui dépasse est laissé de côté. La GPT de secours est déplacée à la nouvelle fin de l&apos;image.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>Lit le MBR ou la GPT du périphérique et laisse de côté l&apos;espace non partitionné entre ses partitions et après elles. Jusqu&apos;à 32 Mo de l&apos;espace qui précède la première partition, où un chargeur d&apos;amorçage peut être stocké dans l&apos;espace inutilisé, sont lus tels quels ; seul ce qui dépasse est laissé de côté. La GPT de secours est déplacée à la nouvelle fin de l&apos;image.</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">Avant la lecture, affiche les partitions du périphérique et permet de choisir lesquelles inclure. Tout ce qui est laissé de côté est retiré de l&apos;image, comme l&apos;espace non partitionné -- l&apos;espace non partitionné est alors toujours ignoré aussi, que « Ignorer l&apos;espace non partitionné » soit cochée ou non.</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>Choisir les partitions à lire</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>Ne lire que certaines partitions du périphérique : ouvre « Partitionnement personnalisé » avec le périphérique comme source, toutes les partitions cochées, et le fichier image comme destination. Décochez ce qu&apos;il faut laisser de côté.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>Ignorer l&apos;espace non partitionné à la lecture</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Outils</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>Vérifier la GPT du périphérique</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>Partitionnement personnalisé...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>Placer des partitions provenant de fichiers image et de disques sur un périphérique, ou dans un nouveau fichier image, dans l&apos;ordre de votre choix, sous une nouvelle table de partitions.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Empreinte du fichier image</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Type d&apos;empreinte à générer pour le fichier image</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Générer l&apos;empreinte sélectionnée du fichier</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Copier l&apos;empreinte dans le presse-papiers</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>Corriger la GPT après l&apos;écriture</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Après l&apos;écriture, déplace la GPT de secours à la fin du périphérique et met l&apos;en-tête à jour en conséquence, afin que Windows n&apos;ait rien à « réparer ». Si la case est décochée, un avertissement vous invitera plutôt à retirer le périphérique.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Afficher tous les périphériques</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Affiche aussi les disques fixes. Les lecteurs de cartes PCIe internes présentent souvent la carte comme un périphérique non amovible, qui reste sinon masqué. Le disque depuis lequel Windows s&apos;exécute n&apos;est jamais listé.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lit le MBR ou la GPT du périphérique et laisse de côté l&apos;espace non partitionné entre ses partitions et après elles. Tout ce qui précède la première partition, où se trouve un chargeur d&apos;amorçage, est lu tel quel, et la première partition ne bouge pas. La GPT de secours est déplacée à la nouvelle fin de l&apos;image.</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>Le format compressé de lecture : .img.zst est le plus rapide, .img.xz le plus petit et .img.gz le plus largement pris en charge</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Progression</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Annuler la tâche en cours.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Lire les données du « périphérique » vers le « fichier image »</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Lire</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Écrire les données du « fichier image » vers le « périphérique »</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Écrire</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Comparer les données du « périphérique » avec le « fichier image »</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Comparer le fichier image avec le lecteur sélectionné</translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Vérifier seulement</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Fermer Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>Fermer ?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Quitter maintenant produira un fichier image corrompu.
Voulez-vous vraiment quitter ?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Quitter maintenant produira un disque corrompu.
Voulez-vous vraiment quitter ?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Sélectionner une image disque</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Génération…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>Annuler ?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Annuler maintenant corrompra la destination.
Voulez-vous vraiment annuler ?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Erreur d&apos;écriture</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>Le fichier image ne peut pas se trouver sur le périphérique cible.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Confirmer l&apos;écrasement</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">En attente d&apos;une tâche.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Quitter maintenant annulera la vérification de l&apos;image.
Voulez-vous vraiment quitter ?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Annuler la vérification.
Voulez-vous vraiment annuler ?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>Espace disponible insuffisant !</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>Erreur de fichier</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Écriture réussie, mais la table de partitions est en danger.

%1
%2

%3

Retirez physiquement le périphérique MAINTENANT, avant toute autre chose, et ne le réinsérez pas dans cet ordinateur. Insérez-le plutôt dans le matériel cible.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>Le fichier sélectionné n&apos;existe pas.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>Le fichier spécifié ne contient aucune donnée.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Effectué.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Terminé</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Écriture réussie.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>Images disque (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>Images disque compressées (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">Impossible d&apos;ouvrir le fichier pour calculer une somme de contrôle :
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Veuillez sélectionner un périphérique cible.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Tous les fichiers et données de cet appareil seront supprimés.
(Périphérique cible : %1)
Voulez-vous vraiment continuer ?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>Le périphérique a des volumes montés</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 est monté dans Windows en tant que %2.

Tout ce qui se trouve sur ce périphérique, sur chacune de ses partitions, sera détruit et ne pourra pas être récupéré.

Vérifiez que %2 n&apos;est pas un lecteur que vous vouliez conserver.

Écrire quand même sur ce périphérique ?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Échec de l&apos;écriture.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Erreur de périphérique</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>Le périphérique indique une taille nulle. S&apos;il s&apos;agit d&apos;un lecteur de cartes, la carte a peut-être été retirée.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>Impossible d&apos;ouvrir le fichier pour calculer une empreinte :
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>Calcul de l&apos;empreinte…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>Impossible de lire l&apos;intégralité du fichier pour calculer une empreinte :
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>Calcul de l&apos;empreinte annulé.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>Taille de l&apos;image supérieure à celle du périphérique :
  Image : au moins %1 secteurs
  Disponible : %2 secteurs
  Taille de secteur : %3

La fin de l&apos;image ne sera pas écrite, le périphérique ne contiendra donc pas une image complète.

Continuer quand même ?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Il faut plus d&apos;espace qu&apos;il n&apos;y en a de disponible :
  Nécessaire : %1 secteurs
  Disponible : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire n&apos;a pas pu être examiné, car l&apos;image est compressée

Continuer quand même ?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Il faut plus d&apos;espace qu&apos;il n&apos;y en a de disponible :
  Nécessaire : %1 secteurs
  Disponible : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire semble BIEN contenir des données

Continuer quand même ?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Il faut plus d&apos;espace qu&apos;il n&apos;y en a de disponible :
  Nécessaire : %1 secteurs
  Disponible : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire ne semble pas contenir de données

Continuer quand même ?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Écriture annulée.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Effacement des anciennes tables de partitions…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>Impossible d&apos;effacer les tables de partitions existantes sur le périphérique.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>Le périphérique n&apos;a été écrit que partiellement et ne contient plus d&apos;image utilisable. Réécrivez l&apos;image avant de l&apos;utiliser.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>Correction de la GPT…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Image tronquée</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">L&apos;image est plus grande que le périphérique : sa fin n&apos;a donc pas été écrite et le périphérique ne contient pas une image complète.

Cela n&apos;a pu être détecté qu&apos;une fois le périphérique plein, car une image gzip n&apos;enregistre pas sa taille décompressée.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Écriture réussie.

La GPT a été mise en cohérence avec le périphérique (%1), si bien que Windows n&apos;a aucune table endommagée à réparer. Le périphérique peut être retiré normalement.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Écriture réussie.

L&apos;image ne contient pas de GPT, il n&apos;y a donc aucune table de partitions que Windows puisse réparer. Le périphérique peut être retiré normalement.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Écriture réussie.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Écriture réussie</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">Le périphérique a été mis hors ligne et éjecté.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">Le périphérique n&apos;a PAS pu être mis hors ligne automatiquement.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">La GPT n&apos;a pas pu être corrigée automatiquement (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">la GPT est mal formée</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>La correction de la GPT a échoué (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>erreur d&apos;écriture</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">L&apos;option « Corriger la GPT après l&apos;écriture » n&apos;est pas activée.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">Cette image EST concernée par le bogue de réécriture de la GPT sous Windows.

Elle réserve de l&apos;espace avant sa première partition : lors d&apos;une nouvelle analyse, Windows réécrit donc la table de partitions primaire de façon à ce qu&apos;elle pointe vers les mauvais secteurs. Le résultat passe toujours les contrôles de Windows, mais Linux le rejette et le périphérique ne démarrera pas.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">Cette image n&apos;est PAS concernée par le bogue de réécriture de la GPT sous Windows.

Windows réécrira tout de même la table lors d&apos;une nouvelle analyse, car la GPT de secours n&apos;est pas à la fin du périphérique, mais avec cette disposition la réécriture tombe sur les bonnes valeurs. Retirer le périphérique maintenant le laisse de toute façon identique à l&apos;image, octet pour octet.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">Impossible de déterminer si cette image est concernée par le bogue de réécriture de la GPT sous Windows. Partez du principe qu&apos;elle l&apos;est : une nouvelle analyse peut faire rejeter la table de partitions par Linux et rendre le périphérique non amorçable.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Retirez le périphérique maintenant</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>Vous n&apos;avez pas la permission de lire le fichier sélectionné.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Les images ne peuvent être relues que non compressées. Choisissez un nom de fichier sans extension .gz ni .xz.

Les images compressées (.img.gz, .img.xz) peuvent être écrites et vérifiées.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Échec de la lecture.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Échec de la vérification.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>Taille de l&apos;image supérieure à celle du périphérique :
  Image : au moins %1 secteurs
  Périphérique : %2 secteurs
  Taille de secteur : %3

Seule la partie qui tient pourra être comparée.

Continuer quand même ?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Taille de l&apos;image supérieure à celle du périphérique :
  Image : %1 secteurs
  Périphérique : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire n&apos;a pas pu être examiné, car l&apos;image est compressée

Continuer quand même ?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Vérification annulée.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Vérification…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Table de partitions endommagée</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Échec de la réparation</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>La table de partitions n&apos;a pas pu être réparée : %1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">Sélectionnez les partitions à inclure dans l&apos;image.</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Image plus grande que le périphérique</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">L&apos;image est plus grande que le périphérique : seule la partie qui tient a donc pu être comparée. Tout ce qui a été comparé correspond, mais le périphérique ne contient pas une image complète.

Cela n&apos;a pu être détecté qu&apos;à la fin du périphérique, car une image gzip n&apos;enregistre pas sa taille décompressée.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Disque %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Veuillez spécifier le fichier image à utiliser.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Analyse des disques…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">Impossible de lire le fichier en entier pour calculer une somme de contrôle :
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>Écriture : %1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>Lecture : %1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>Vérification : %1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>Calcul de l&apos;empreinte : %1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">Génération de la somme de contrôle…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">Somme de contrôle annulée.</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1 l&apos;en-tête GPT primaire pointe vers des secteurs où ne se trouvent pas les entrées de partition.

C&apos;est ce que laisse Windows lorsqu&apos;il réanalyse une carte écrite sans « Corriger la GPT après l&apos;écriture ». Aucune donnée n&apos;a été perdue, mais le périphérique ne démarrera pas et la plupart des outils refuseront la table.

Réparer la table de partitions maintenant ?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Veuillez sélectionner un périphérique.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>Impossible de verrouiller le périphérique.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>Impossible d&apos;ouvrir le périphérique.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>La table de partitions de ce périphérique est endommagée :</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Table de partitions réparée.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>La table de partitions est toujours endommagée.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>La table de partitions est valide.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Table de partitions</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>La GPT de ce périphérique est valide : l&apos;en-tête et les entrées de partition vers lesquelles il pointe concordent.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>Aucune GPT sur ce périphérique.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>Ce périphérique n&apos;a pas de GPT ; il ne peut donc pas présenter le dommage recherché ici.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>Impossible de lire la table de partitions.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>La table de partitions n&apos;a pas pu être lue, ou elle est endommagée d&apos;une manière autre que celle que cette fonction répare.</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>Le périphérique cible est aussi l&apos;une des sources.</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 se trouve sur le périphérique cible et ne peut pas y être écrit.</translation>
    </message>
    <message>
        <source>Confirm write</source>
        <translation>Confirmer l&apos;écriture</translation>
    </message>
    <message>
        <source>The device keeps the partitions ticked in the order, and they are not written to. Anything in its free space, and in the partitions taken out of its table, may be overwritten.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Le périphérique conserve les partitions cochées dans l&apos;ordre, et elles ne sont pas écrites. Tout ce qui se trouve dans son espace libre, et dans les partitions retirées de sa table, peut être écrasé.
(Périphérique cible : %1)
Voulez-vous vraiment continuer ?</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Its volumes are dismounted while it is written, and the device is ejected afterwards. The kept partitions are not changed.

Write to this device anyway?</source>
        <translation>%1 est monté dans Windows en tant que %2.

Ses volumes sont démontés pendant l&apos;écriture, et le périphérique est éjecté ensuite. Les partitions conservées ne sont pas modifiées.

Écrire quand même sur ce périphérique ?</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>La liste des périphériques a changé pendant la confirmation. Vérifiez le périphérique cible et réessayez.</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 se termine au secteur %2, avant la fin de la partition qu&apos;il doit y fournir : l&apos;image est incomplète.</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>Le secteur %1 du périphérique ne correspond pas au secteur %2 de %3.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>Le périphérique n&apos;a été écrit que partiellement et ne contient plus de disposition utilisable. Réécrivez-le avant de l&apos;utiliser.</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has changed since Custom Partitioning read it. Open Custom Partitioning again to plan from what the device holds now.</source>
        <translation>La table de partitions du périphérique a changé depuis que « Partitionnement personnalisé » l&apos;a lue. Rouvrez « Partitionnement personnalisé » pour planifier à partir de ce que contient maintenant le périphérique.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Écriture…</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has not been changed: it holds its partitions as before. Its free space may hold part of the new ones.</source>
        <translation>La table de partitions du périphérique n&apos;a pas été modifiée : il contient ses partitions comme avant. Son espace libre peut contenir une partie des nouvelles.</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>La table de partitions du périphérique ne correspond pas à ce qui a été écrit.</translation>
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
        <translation>La table de partitions %1 du périphérique contient maintenant %2 partitions : %3 conservées et %4 nouvelles provenant de %5 images.</translation>
    </message>
    <message>
        <source>Write and verify successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Écriture et vérification réussies.

Le périphérique contient une nouvelle table de partitions %1 avec %2 partitions provenant de %3 images.</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Écriture réussie.

Le périphérique contient une nouvelle table de partitions %1 avec %2 partitions provenant de %3 images.</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>Sa copie de secours se trouve déjà à la fin du périphérique, Windows n&apos;a donc rien à réparer.</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>Son démarrage dépend de la capacité de ses chargeurs d&apos;amorçage à trouver leurs partitions à leur nouvel emplacement.</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>Partitionnement personnalisé</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>L&apos;image est plus grande que le périphérique : sa fin n&apos;a donc pas été écrite et le périphérique ne contient pas une image complète.

Cela n&apos;a pu être détecté qu&apos;une fois le périphérique plein, car l&apos;image compressée n&apos;enregistre pas sa taille décompressée.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Écriture réussie.

La GPT correspond maintenant au périphérique (%1), Windows n&apos;a donc rien à réparer. Retirez le périphérique normalement.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Écriture réussie.

Cette image utilise une table de partitions MBR et non GPT ; le bogue de réécriture GPT de Windows ne peut donc pas l&apos;affecter. Retirez le périphérique normalement.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Écriture réussie.

Cette image n&apos;a pas de table de partitions ; le bogue de réécriture GPT de Windows ne peut donc pas l&apos;affecter. Retirez le périphérique normalement.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>Le périphérique est hors ligne et éjecté.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>Le périphérique n&apos;a PAS pu être mis hors ligne.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>La GPT n&apos;a pas pu être corrigée (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>GPT mal formée</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>« Corriger la GPT après l&apos;écriture » est désactivé.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Cette image EST affectée : elle réserve de l&apos;espace avant sa première partition, de sorte qu&apos;une nouvelle analyse fait pointer la table primaire vers les mauvais secteurs. Windows accepte encore le résultat, Linux non, et le périphérique ne démarrera pas.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Cette image n&apos;est PAS affectée : une nouvelle analyse réécrit toujours la table, mais pour cette disposition elle écrit les bonnes valeurs. Retirer le périphérique maintenant le laisse de toute façon identique à l&apos;image.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>Impossible de déterminer si cette image est affectée. Partez du principe que oui : une nouvelle analyse peut laisser une table que Linux refuse et le périphérique ne démarrera pas.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Écriture réussie, mais la table de partitions est menacée.

%1 %2

%3

Retirez le périphérique MAINTENANT et ne le réinsérez pas ici. Placez-le directement dans le matériel cible.</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>L&apos;image combinée s&apos;est terminée prématurément.</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>Le secteur %1 de %2 ne correspond pas à ce qui a été écrit.</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 contient plus que l&apos;image combinée, ou ne se termine pas proprement.</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>Écriture et vérification réussies.</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 contient une table de partitions %2 avec %3 partitions provenant de %4 images.</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>Sa GPT de secours termine l&apos;image ; « Corriger la GPT après l&apos;écriture » la déplace à la fin d&apos;un périphérique plus grand lors de l&apos;écriture de l&apos;image.</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">Choisir les partitions</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">Ignorer l&apos;espace non partitionné ne conserve que les partitions et la table de partitions, ainsi que l&apos;espace qu&apos;une GPT réserve avant ses partitions.

Certaines images amorçables, comme celles des ordinateurs monocartes, placent des données du chargeur d&apos;amorçage hors des partitions. Une image d&apos;un tel périphérique lue de cette façon risque de ne pas démarrer.</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">La réduction ne conserve que les partitions et la table de partitions, ainsi que l&apos;espace qu&apos;une GPT réserve avant ses partitions.

Certaines images amorçables, comme celles des ordinateurs monocartes, placent des données du chargeur d&apos;amorçage hors des partitions. Une image réduite d&apos;un tel périphérique risque de ne pas démarrer.</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">Choisissez les partitions à inclure dans l&apos;image. Tout ce qui reste décoché est retiré, comme l&apos;espace non partitionné.</translation>
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
        <translation type="vanished">Au moins une partition doit rester cochée.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Erreur de lecture</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">Les images ne peuvent être relues que non compressées. Choisissez un nom de fichier sans extension .gz ou .xz, ou cochez « Lire vers .img.gz » ou « Lire vers .img.xz ».</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Veuillez sélectionner un périphérique source.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Confirmer l&apos;écrasement</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>Voulez-vous vraiment écraser le fichier spécifié ?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">Aucune table de partitions n&apos;a été trouvée sur le périphérique, il n&apos;y a donc rien à choisir. L&apos;intégralité du périphérique sera lue.</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">Lecture annulée.</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>Le disque n&apos;est pas assez grand pour contenir l&apos;image spécifiée.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Lecture…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Lecture annulée.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Lecture réussie.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>Infos sur le fichier</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Veuillez spécifier un fichier dans lequel enregistrer les données.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Erreur de vérification</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Veuillez sélectionner un périphérique auquel comparer.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Taille de l&apos;image supérieure à celle du périphérique :
  Image : %1 secteurs
  Périphérique : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire semble BIEN contenir des données

Continuer quand même ?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Taille de l&apos;image supérieure à celle du périphérique :
  Image : %1 secteurs
  Périphérique : %2 secteurs
  Taille de secteur : %3

L&apos;espace excédentaire ne semble pas contenir de données

Continuer quand même ?</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>Le périphérique n&apos;a pas pu être lu au secteur %1.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>Le périphérique contient correctement l&apos;image, mais sa table de partitions est endommagée :</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>Le périphérique contient correctement l&apos;image, mais sa table de partitions est toujours endommagée. Réécrivez l&apos;image avec l&apos;option « Corriger la GPT après l&apos;écriture » cochée, ou relancez la vérification et acceptez la réparation.</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>Tailles différentes !</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Échec de la vérification</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>Échec de la vérification au secteur : %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>L&apos;image est plus grande que le périphérique : seule la partie qui tient a donc pu être comparée. Tout ce qui a été comparé correspond, mais le périphérique ne contient pas une image complète.

Cela n&apos;a pu être détecté qu&apos;à la fin du périphérique, car l&apos;image compressée n&apos;enregistre pas sa taille décompressée.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Vérification réussie.

L&apos;image et le périphérique ne diffèrent que par la GPT, que l&apos;option « Corriger la GPT après l&apos;écriture » réécrit volontairement.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Vérification réussie.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Vérification réussie.

La table de partitions du périphérique était endommagée et a été réparée.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Vérification réussie.

L&apos;image et le périphérique ne diffèrent que par la GPT, et la GPT du périphérique est valide.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Vérification réussie.

L&apos;image et le périphérique ne diffèrent que par la GPT.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

Le périphérique a été éjecté. Retirez-le maintenant.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

Le périphérique n&apos;a PAS pu être mis hors ligne automatiquement.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>Erreur de fichier</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative d&apos;accès au fichier.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Erreur de périphérique</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative d&apos;accès au périphérique.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Échec de l&apos;obtention de l&apos;espace libre sur le volume contenant %1.
Erreur %2 : %3
La vérification de l&apos;espace libre ne sera pas effectuée.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Erreur de verrouillage</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">Une erreur est apparue lors de la tentative de verrouillage du volume.
Erreur %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Erreur de déverrouillage</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative de déverrouillage du volume.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Erreur de démontage</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative de démontage du volume.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Erreur de lecture</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>Le nombre de secteurs est trop élevé.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>Impossible d&apos;allouer la mémoire pour le tampon de lecture.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative de lecture des données.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Erreur d&apos;écriture</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative d&apos;écriture des données.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>Le périphérique n&apos;a accepté que %1 octets sur %2. L&apos;image sur le périphérique est incomplète.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>Une erreur est apparue lors de la tentative d&apos;obtention de la géométrie du périphérique.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Une erreur s&apos;est produite lors de l&apos;obtention de la taille du fichier.
Erreur %1 : %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Erreur d&apos;espace libre</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Echec d&apos;obtention de l&apos;espace libre sur le disque %1.
Erreur %2: %3
La vérification de l&apos;espace libre ne sera pas effectuée.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Périphérique inconnu</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>Impossible de lister les volumes de cet ordinateur.
Erreur %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>Impossible de verrouiller le volume %1 : il est encore utilisé.
Fermez tout programme utilisant le périphérique et réessayez.
Erreur %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>la taille de l&apos;en-tête GPT primaire est hors limites</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>la somme de contrôle de l&apos;en-tête GPT primaire est invalide</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1, aucune table de partitions</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>système de fichiers non reconnu, aucune table de partitions</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>la taille de l&apos;en-tête GPT est hors limites</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>la somme de contrôle de l&apos;en-tête GPT est invalide</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>le tableau des entrées de partition GPT ne se trouve pas là où l&apos;en-tête l&apos;indique</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA se trouve à l&apos;intérieur de la table de partitions</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>la partition %1 dépasse la fin de l&apos;image</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>la partition %1 décrit une plage impossible</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>la GPT ne contient aucune partition</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>deux partitions se chevauchent</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>étendue, avec ses partitions logiques (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>type 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>le MBR ne contient aucune partition</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>le MBR a plus d&apos;une partition étendue</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>l&apos;image est plus petite qu&apos;un secteur</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>l&apos;image a un MBR de protection mais pas d&apos;en-tête GPT</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>une partition MBR étendue ne peut pas aller sur une GPT : choisissez l&apos;image des partitions logiques comme zone de démarrage, ou laissez-la de côté</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>le type de partition MBR 0x%1 n&apos;a pas d&apos;équivalent GPT connu de ce programme</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>le type de partition GPT %1 n&apos;a pas d&apos;équivalent MBR</translation>
    </message>
    <message>
        <source>the device has no partition table to keep</source>
        <translation>le périphérique n&apos;a aucune table de partitions à conserver</translation>
    </message>
    <message>
        <source>a lead-in cannot be used while the device keeps its own partitions</source>
        <translation>une zone de démarrage ne peut pas être utilisée lorsque le périphérique conserve ses propres partitions</translation>
    </message>
    <message>
        <source>only a device can keep its own partitions</source>
        <translation>seul un périphérique peut conserver ses propres partitions</translation>
    </message>
    <message>
        <source>a free space has no size</source>
        <translation>un espace libre n&apos;a pas de taille</translation>
    </message>
    <message>
        <source>the device&apos;s own partitions must stay in the order they are on it</source>
        <translation>les partitions propres au périphérique doivent rester dans l&apos;ordre où elles s&apos;y trouvent</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>aucune partition n&apos;est choisie</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>une partition choisie n&apos;existe pas</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>une partition est choisie deux fois</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>la taille d&apos;une image sans table de partitions n&apos;est pas connue : analysez-la d&apos;abord</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>l&apos;image de la zone de démarrage n&apos;a pas de table de partitions</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>un MBR contient au plus quatre partitions, et %1 sont choisies</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>un MBR ne peut contenir qu&apos;une seule partition étendue</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>la GPT a de la place pour %1 partitions, et %2 sont choisies</translation>
    </message>
    <message>
        <source>there is %1 MB too little free space before the device&apos;s partition %2, which is kept</source>
        <translation>il manque %1 MB d&apos;espace libre avant la partition %2 du périphérique, qui est conservée</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>la disposition ne tient plus dans une entrée MBR 32 bits</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>les partitions nécessitent %1 MB et le périphérique en a %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>le tableau des entrées GPT ne tient pas sur le périphérique</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>la somme de contrôle du tableau des entrées de partition GPT est invalide</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>une partition dépasse la fin du périphérique</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>GPT de secours déplacée vers le LBA %1 ; le dernier LBA utilisable est maintenant %2 ; l&apos;ancienne copie au LBA %3 a été effacée</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>GPT de secours déplacée vers le LBA %1 ; le dernier LBA utilisable est maintenant %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>le périphérique a une GPT, que son MBR ne fait que refléter</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>le MBR ne contient aucune partition vers laquelle réduire</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>la disposition réempaquetée ne tient plus dans une entrée MBR 32 bits</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>le périphérique est déjà aussi compact ; rien à réduire</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA n&apos;est pas utilisable pour le réempaquetage</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>la GPT ne contient aucune partition vers laquelle réduire</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>une entrée de partition décrit une plage impossible</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>la géométrie du périphérique est inutilisable</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>l&apos;en-tête GPT primaire n&apos;est pas lisible</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>la géométrie du tableau des entrées GPT est inutilisable</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>le périphérique est trop petit pour contenir un tableau d&apos;entrées</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>les entrées de partition n&apos;ont pas pu être lues</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>les entrées de partition ne sont pas au LBA 2 ; il ne s&apos;agit donc pas du dommage que cette fonction peut réparer</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>l&apos;en-tête réparé n&apos;a pas pu être écrit</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA pointe de nouveau vers le LBA 2 et la somme de contrôle de l&apos;en-tête a été recalculée</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>Le périphérique indique une taille de secteur nulle.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>Le disque %1 n&apos;a pas pu être ouvert (erreur %2).</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>La taille du disque %1 n&apos;a pas pu être lue (erreur %2).</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>Le disque %1 a des secteurs de %2 octets, et non de %3.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être ouvert (erreur %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>La taille du fichier image n&apos;a pas pu être lue (erreur %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être lu (erreur %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être rembobiné (erreur %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>Le décompresseur bzip2 n&apos;a pas pu être démarré (erreur bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>Le décompresseur zstd n&apos;a pas pu être démarré (erreur zstd %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>Le décompresseur gzip n&apos;a pas pu être démarré (erreur zlib %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>Le décompresseur xz n&apos;a pas pu être démarré (erreur lzma %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>Le fichier image s&apos;arrête au milieu des données compressées. Il est tronqué ou endommagé.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>L&apos;image gzip n&apos;a pas pu être décompressée.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>L&apos;image gzip est endommagée (erreur zlib %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>L&apos;image bzip2 n&apos;a pas pu être décompressée.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>L&apos;image bzip2 est endommagée (erreur bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>L&apos;image zstd est endommagée (erreur zstd %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>L&apos;image xz est endommagée (erreur lzma %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>Une image compressée ne peut être lue que vers l&apos;avant.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être créé (erreur %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>Le compresseur gzip n&apos;a pas pu être démarré (erreur zlib %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>Le compresseur xz n&apos;a pas pu être démarré (erreur lzma %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>Le compresseur bzip2 n&apos;a pas pu être démarré (erreur bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>Le compresseur zstd n&apos;a pas pu être démarré (erreur zstd %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>Le compresseur gzip a échoué (erreur zlib %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>Le compresseur xz a échoué (erreur lzma %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>Le compresseur bzip2 a échoué (erreur bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>Le compresseur zstd a échoué (erreur zstd %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être écrit (erreur %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>Le fichier image n&apos;est pas ouvert en écriture.</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>Le fichier image n&apos;a pas pu être vidé sur le disque (erreur %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 se termine au secteur %2, avant la fin de la partition qu&apos;il doit y fournir : l&apos;image est incomplète.</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>Disque %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>Les disques sources vont être démontés</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>Pendant leur lecture, les volumes de ces disques sources sont verrouillés et démontés, afin que rien ne les modifie en cours de route :

%1

Les programmes qui les utilisent les perdent jusqu&apos;à la fin de l&apos;opération. Rien n&apos;y est modifié. Continuer ?</translation>
    </message>
</context>
</TS>
