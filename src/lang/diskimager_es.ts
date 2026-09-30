<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es">
<context>
    <name>CombineDialog</name>
    <message>
        <source>Custom Partitioning</source>
        <translation>Particionado personalizado</translation>
    </message>
    <message>
        <source>Add image files or disks, tick the partitions to put on the device or in a new image file, and order them. Each source&apos;s partition table is read from its first sectors; nothing else is read until you write, or ask for a full scan.</source>
        <translation>Añada archivos de imagen o discos, marque las particiones que quiere poner en el dispositivo o en un nuevo archivo de imagen y ordénelas. La tabla de particiones de cada origen se lee de sus primeros sectores; no se lee nada más hasta que escriba o pida un análisis completo.</translation>
    </message>
    <message>
        <source>Sources</source>
        <translation>Orígenes</translation>
    </message>
    <message>
        <source>Source / partition</source>
        <translation>Origen / partición</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>Add images...</source>
        <translation>Añadir imágenes...</translation>
    </message>
    <message>
        <source>Add disks...</source>
        <translation>Añadir discos...</translation>
    </message>
    <message>
        <source>Take partitions from disks as well: cards, USB drives, and other disks. The disk Windows runs from is never offered. While a disk is read, its volumes are locked and dismounted.</source>
        <translation>Tomar particiones también de discos: tarjetas, unidades USB y otros discos. Nunca se ofrece el disco desde el que se ejecuta Windows. Mientras se lee un disco, sus volúmenes se bloquean y se desmontan.</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Quitar</translation>
    </message>
    <message>
        <source>Full scan</source>
        <translation>Análisis completo</translation>
    </message>
    <message>
        <source>Read and decompress the whole image, to learn its exact size and check that it holds every partition to its end. Only needed for an image with no partition table whose size the file does not record, or to check a compressed image before writing.</source>
        <translation>Lee y descomprime la imagen entera para conocer su tamaño exacto y comprobar que contiene cada partición hasta su final. Solo es necesario para una imagen sin tabla de particiones cuyo archivo no indica el tamaño, o para comprobar una imagen comprimida antes de escribirla.</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Disposición</translation>
    </message>
    <message>
        <source>Partitions, in order:</source>
        <translation>Particiones, en orden:</translation>
    </message>
    <message>
        <source>Up</source>
        <translation>Subir</translation>
    </message>
    <message>
        <source>Down</source>
        <translation>Bajar</translation>
    </message>
    <message>
        <source>Free space:</source>
        <translation>Espacio libre:</translation>
    </message>
    <message>
        <source> MiB</source>
        <translation> MiB</translation>
    </message>
    <message>
        <source>How much unpartitioned space to insert, or, with free space selected in the order, how much it is. The partition after it still starts on a 1 MiB boundary.</source>
        <translation>Cuánto espacio sin particionar insertar o, con un espacio libre seleccionado en el orden, cuánto mide. La partición siguiente sigue empezando en un límite de 1 MiB.</translation>
    </message>
    <message>
        <source>Insert</source>
        <translation>Insertar</translation>
    </message>
    <message>
        <source>Leave this much unpartitioned space after the selected item of the order, or at the end.</source>
        <translation>Dejar esta cantidad de espacio sin particionar después del elemento seleccionado del orden, o al final.</translation>
    </message>
    <message>
        <source>Remove free space</source>
        <translation>Quitar espacio libre</translation>
    </message>
    <message>
        <source>Lead-in from:</source>
        <translation>Zona de arranque de:</translation>
    </message>
    <message>
        <source>Copy this image&apos;s boot code, and the space between its partition table and its first partition (up to 32 MiB), where a bootloader may be stored. The device then gets the same kind of partition table as this image, and the first partition starts where this image&apos;s did.</source>
        <translation>Copia el código de arranque de esta imagen y el espacio entre su tabla de particiones y su primera partición (hasta 32 MiB), donde puede guardarse un gestor de arranque. El dispositivo recibe entonces el mismo tipo de tabla de particiones que esta imagen, y la primera partición empieza donde empezaba la de esta imagen.</translation>
    </message>
    <message>
        <source>On the device</source>
        <translation>En el dispositivo</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Inicio</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Origen</translation>
    </message>
    <message>
        <source>Write to</source>
        <translation>Escribir en</translation>
    </message>
    <message>
        <source>A device:</source>
        <translation>Un dispositivo:</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Mostrar todos los dispositivos</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Muestra también los discos fijos. Los lectores de tarjetas PCIe internos suelen presentar la tarjeta como un dispositivo no extraíble, que de otro modo queda oculto. Nunca se muestra el disco desde el que se ejecuta Windows.</translation>
    </message>
    <message>
        <source>Keep the device&apos;s partitions</source>
        <translation>Conservar las particiones del dispositivo</translation>
    </message>
    <message>
        <source>Add to what the device holds instead of replacing it: its partitions stay where they are, untouched, and the new ones go into its free space, where the order puts them. Untick one of them in the order to take it out of the table; its space is then free. The table keeps its kind, and its backup GPT is moved to the end of the device.</source>
        <translation>Añade a lo que contiene el dispositivo en lugar de reemplazarlo: sus particiones se quedan donde están, sin tocar, y las nuevas van a su espacio libre, donde las pone el orden. Desmarque una de ellas en el orden para sacarla de la tabla; su espacio queda entonces libre. La tabla conserva su tipo, y su GPT de respaldo se mueve al final del dispositivo.</translation>
    </message>
    <message>
        <source>An image file:</source>
        <translation>Un archivo de imagen:</translation>
    </message>
    <message>
        <source>combined.img</source>
        <translation type="vanished">combined.img</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Examinar...</translation>
    </message>
    <message>
        <source>Compress to</source>
        <translation>Comprimir en</translation>
    </message>
    <message>
        <source>The compressed format to write to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>El formato comprimido de escritura: .img.zst es el más rápido, .img.xz el más pequeño y .img.gz el más compatible</translation>
    </message>
    <message>
        <source>Verify after writing</source>
        <translation>Verificar después de escribir</translation>
    </message>
    <message>
        <source>Write...</source>
        <translation>Escribir...</translation>
    </message>
    <message>
        <source>no device is chosen to write to</source>
        <translation>no se ha elegido ningún dispositivo en el que escribir</translation>
    </message>
    <message>
        <source>disk %1 could not be read</source>
        <translation>no se pudo leer el disco %1</translation>
    </message>
    <message>
        <source>disk %1 has %2-byte sectors, and the sources %3-byte ones</source>
        <translation>el disco %1 tiene sectores de %2 bytes, y los orígenes de %3 bytes</translation>
    </message>
    <message>
        <source>Disk %1: %2</source>
        <translation>Disco %1: %2</translation>
    </message>
    <message>
        <source>disk %1 could not be read: %2</source>
        <translation>no se pudo leer el disco %1: %2</translation>
    </message>
    <message>
        <source>disk %1 has no partition table to keep</source>
        <translation>el disco %1 no tiene ninguna tabla de particiones que conservar</translation>
    </message>
    <message>
        <source>Save the combined image as</source>
        <translation>Guardar la imagen combinada como</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</source>
        <translation>Imágenes de disco (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)</translation>
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
        <translation>la imagen termina dentro de su propia tabla de particiones</translation>
    </message>
    <message>
        <source>the partition table could not be read</source>
        <translation>no se pudo leer la tabla de particiones</translation>
    </message>
    <message>
        <source>Add images</source>
        <translation>Añadir imágenes</translation>
    </message>
    <message>
        <source>%1 cannot be used: %2.</source>
        <translation>%1 no se puede usar: %2.</translation>
    </message>
    <message>
        <source>%1 has no partition table, so it is taken as one partition: the whole image. The file does not record how big that is, so it has to be read to the end to find out.

Scan it now?</source>
        <translation>%1 no tiene tabla de particiones, así que se toma como una sola partición: la imagen entera. El archivo no indica su tamaño, así que hay que leerlo hasta el final para averiguarlo.

¿Analizarlo ahora?</translation>
    </message>
    <message>
        <source>Add disks</source>
        <translation>Añadir discos</translation>
    </message>
    <message>
        <source>Tick the disks to take partitions from:</source>
        <translation>Marque los discos de los que tomar particiones:</translation>
    </message>
    <message>
        <source>Also list fixed disks. The disk Windows is running from is never listed.</source>
        <translation>Muestra también los discos fijos. Nunca se muestra el disco desde el que se ejecuta Windows.</translation>
    </message>
    <message>
        <source> -- the device being written to</source>
        <translation> -- el dispositivo en el que se escribe</translation>
    </message>
    <message>
        <source>Already a source.</source>
        <translation>Ya es un origen.</translation>
    </message>
    <message>
        <source>Disk %1 cannot be used: %2.</source>
        <translation>El disco %1 no se puede usar: %2.</translation>
    </message>
    <message>
        <source>disk</source>
        <translation>disco</translation>
    </message>
    <message>
        <source>Scanning %1...</source>
        <translation>Analizando %1…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>%1 could not be read to the end: %2</source>
        <translation>No se pudo leer %1 hasta el final: %2</translation>
    </message>
    <message>
        <source>Scanning %1: %2 read...</source>
        <translation>Analizando %1: %2 leídos…</translation>
    </message>
    <message>
        <source>%1 ends at %2, before its partition %3 does: the image is incomplete, and that partition cannot be copied whole.</source>
        <translation>%1 termina en %2, antes de que termine su partición %3: la imagen está incompleta y esa partición no se puede copiar entera.</translation>
    </message>
    <message>
        <source>whole image</source>
        <translation>imagen entera</translation>
    </message>
    <message>
        <source>Partition %1</source>
        <translation>Partición %1</translation>
    </message>
    <message>
        <source>Partition %1: %2</source>
        <translation>Partición %1: %2</translation>
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
        <translation>sin tabla de particiones</translation>
    </message>
    <message>
        <source>size not recorded</source>
        <translation>tamaño no indicado</translation>
    </message>
    <message>
        <source>%1, scanned</source>
        <translation>%1, analizado</translation>
    </message>
    <message>
        <source>unknown: scan the image</source>
        <translation>desconocido: analice la imagen</translation>
    </message>
    <message>
        <source>None: a new, empty table</source>
        <translation>Ninguna: una tabla nueva y vacía</translation>
    </message>
    <message>
        <source>Free space: %1 MiB</source>
        <translation>Espacio libre: %1 MiB</translation>
    </message>
    <message>
        <source>This device, %1</source>
        <translation>Este dispositivo, %1</translation>
    </message>
    <message>
        <source>%1 -- taken out of the table</source>
        <translation>%1 -- sacada de la tabla</translation>
    </message>
    <message>
        <source>%1 -- kept</source>
        <translation>%1 -- conservada</translation>
    </message>
    <message>
        <source>Tick the partitions to put on the device.</source>
        <translation>Marque las particiones que quiere poner en el dispositivo.</translation>
    </message>
    <message>
        <source>This cannot be written: %1.</source>
        <translation>No se puede escribir: %1.</translation>
    </message>
    <message>
        <source>This cannot be written: %1 is the device being written to. Write to an image file, or choose another device.</source>
        <translation>No se puede escribir: %1 es el dispositivo en el que se escribe. Escriba en un archivo de imagen o elija otro dispositivo.</translation>
    </message>
    <message>
        <source>Partition table (%1)</source>
        <translation>Tabla de particiones (%1)</translation>
    </message>
    <message>
        <source>Lead-in</source>
        <translation>Zona de arranque</translation>
    </message>
    <message>
        <source>Free space</source>
        <translation>Espacio libre</translation>
    </message>
    <message>
        <source>%1, kept</source>
        <translation>%1, conservada</translation>
    </message>
    <message>
        <source>Backup GPT</source>
        <translation>GPT de respaldo</translation>
    </message>
    <message>
        <source>%1, %2 partitions: an image file of %3.</source>
        <translation>%1, %2 particiones: un archivo de imagen de %3.</translation>
    </message>
    <message>
        <source>%1, %2 partitions, %3 of them kept: %4 used, %5 free of %6.</source>
        <translation>%1, %2 particiones, %3 de ellas conservadas: %4 usados, %5 libres de %6.</translation>
    </message>
    <message>
        <source>%1, %2 partitions: %3 used, %4 free of %5.</source>
        <translation>%1, %2 particiones: %3 usados, %4 libres de %5.</translation>
    </message>
    <message>
        <source>Images of unrecorded size are checked only when scanned or written.</source>
        <translation>Las imágenes de tamaño no indicado solo se comprueban al analizarlas o escribirlas.</translation>
    </message>
    <message>
        <source>Some partitions share a GUID: you will be asked about it.</source>
        <translation>Algunas particiones comparten un GUID: se le preguntará al respecto.</translation>
    </message>
    <message>
        <source>Name the image file to write.</source>
        <translation>Indique el nombre del archivo de imagen que se escribirá.</translation>
    </message>
    <message>
        <source>%1 is one of the images being combined; choose another name.</source>
        <translation>%1 es una de las imágenes que se combinan; elija otro nombre.</translation>
    </message>
    <message>
        <source>%1 already exists. Overwrite it?</source>
        <translation>%1 ya existe. ¿Sobrescribirlo?</translation>
    </message>
    <message>
        <source>%1 is on disk %2, which is one of the sources: its volumes are locked while it is read, so nothing can be written to them. Choose a place on another disk.</source>
        <translation>%1 está en el disco %2, que es uno de los orígenes: sus volúmenes se bloquean mientras se lee, así que no se puede escribir nada en ellos. Elija una ubicación en otro disco.</translation>
    </message>
    <message>
        <source>Duplicate partition GUIDs</source>
        <translation>GUID de partición duplicados</translation>
    </message>
    <message>
        <source>These unique partition GUIDs belong to more than one of the chosen partitions:

%1

The copies are usually the same partition taken from two copies of one image. With duplicate GUIDs a system that finds its partitions by PARTUUID -- in fstab or on the kernel command line -- may use the wrong one.

New GUIDs can be generated for the later copies; the first keeps its own. Anything that names a regenerated partition by its old PARTUUID will then no longer find it.</source>
        <translation>Estos GUID únicos de partición pertenecen a más de una de las particiones elegidas:

%1

Normalmente se trata de la misma partición tomada de dos copias de una misma imagen. Con GUID duplicados, un sistema que encuentra sus particiones por PARTUUID -- en fstab o en la línea de comandos del kernel -- puede usar la equivocada.

Se pueden generar GUID nuevos para las copias posteriores; la primera conserva el suyo. Todo lo que haga referencia a una partición regenerada por su PARTUUID anterior dejará de encontrarla.</translation>
    </message>
    <message>
        <source>Generate new GUIDs</source>
        <translation>Generar GUID nuevos</translation>
    </message>
    <message>
        <source>Keep them</source>
        <translation>Conservarlos</translation>
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
        <translation>Archivo de imagen</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Verify</source>
        <translation>Verificar</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <source>Shrink image on Read</source>
        <translation type="vanished">Reducir imagen al leer</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device to shrink the image to match actual partitions only. Moves backup GPT to end of used space.</source>
        <translation type="vanished">Lee el MBR o la GPT del dispositivo para reducir la imagen y que coincida solo con las particiones reales. Mueve la GPT de respaldo al final del espacio utilizado.</translation>
    </message>
    <message>
        <source>Read to .img.gz</source>
        <translation type="vanished">Leer a .img.gz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with gz</source>
        <translation type="vanished">Comprime la imagen leída del dispositivo con gz</translation>
    </message>
    <message>
        <source>Read to .img.xz</source>
        <translation type="vanished">Leer a .img.xz</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device with xz</source>
        <translation type="vanished">Comprime la imagen leída del dispositivo con xz</translation>
    </message>
    <message>
        <source>Choose partitions to read</source>
        <translation type="vanished">Elegir particiones a leer</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always shrinks the image, whether or not &quot;Shrink image on Read&quot; is also checked.</source>
        <translation type="vanished">Antes de leer, muestra las particiones del dispositivo y permite elegir cuáles incluir. Todo lo que se deje fuera se elimina de la imagen, igual que el espacio sin particionar -- esto siempre reduce la imagen, esté o no marcada también la opción «Reducir imagen al leer».</translation>
    </message>
    <message>
        <source>Exit WinDiskImager</source>
        <translation>Salir de WinDiskImager</translation>
    </message>
    <message>
        <source>Exit Win Disk Imager</source>
        <translation type="vanished">Salir de Win Disk Imager</translation>
    </message>
    <message>
        <source>Check GPT</source>
        <translation type="vanished">Comprobar la GPT</translation>
    </message>
    <message>
        <source>Win Disk Imager</source>
        <translation type="vanished">Win Disk Imager</translation>
    </message>
    <message>
        <source>Check the currently selected device for GPT corruption and offer to repair it.</source>
        <translation>Comprueba si la GPT del dispositivo seleccionado está dañada y ofrece repararla.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space</source>
        <translation type="vanished">Omitir espacio sin particionar</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves its unpartitioned space out of the image, keeping the partitions, the partition table and any space a GPT reserves ahead of its partitions. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lee el MBR o la GPT del dispositivo y deja fuera de la imagen el espacio sin particionar, conservando las particiones, la tabla de particiones y el espacio que una GPT reserve antes de sus particiones. La GPT de respaldo se mueve al nuevo final de la imagen.</translation>
    </message>
    <message>
        <source>Compress during Read</source>
        <translation>Comprimir al leer</translation>
    </message>
    <message>
        <source>Compress the Image Read from the Device, in the format chosen below</source>
        <translation>Comprime la imagen leída del dispositivo en el formato elegido abajo</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.gz is faster to make, .img.xz is smaller</source>
        <translation type="vanished">El formato comprimido de lectura: .img.gz se crea más rápido, .img.xz ocupa menos</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. The space before the first partition, where a bootloader is kept, is read as it is up to 32 MB after the partition table; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lee el MBR o la GPT del dispositivo y deja fuera el espacio sin particionar entre sus particiones y después de ellas. El espacio anterior a la primera partición, donde se guarda el gestor de arranque, se lee tal cual hasta 32 MB después de la tabla de particiones; solo se omite lo que haya más allá. La GPT de respaldo se mueve al nuevo final de la imagen.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Up to 32 MB of the space before the first partition, where a bootloader might be stored in unused space, is read as it is; only space beyond that is left out. The backup GPT is moved to the new end of the image.</source>
        <translation>Lee el MBR o la GPT del dispositivo y deja fuera el espacio sin particionar entre sus particiones y después de ellas. Hasta 32 MB del espacio anterior a la primera partición, donde puede haber un gestor de arranque guardado en espacio sin usar, se leen tal cual; solo se omite lo que haya más allá. La GPT de respaldo se mueve al nuevo final de la imagen.</translation>
    </message>
    <message>
        <source>Before reading, list the Device&apos;s partitions and choose which to include. Anything left out is removed from the image, the same as unpartitioned space -- this always skips unpartitioned space too, whether or not &quot;Skip unpartitioned space&quot; is also checked.</source>
        <translation type="vanished">Antes de leer, muestra las particiones del dispositivo y permite elegir cuáles incluir. Todo lo que se deje fuera se elimina de la imagen, igual que el espacio sin particionar -- esto siempre omite también el espacio sin particionar, esté o no marcada la opción «Omitir espacio sin particionar».</translation>
    </message>
    <message>
        <source>Choose Partitions to Read</source>
        <translation>Elegir particiones a leer</translation>
    </message>
    <message>
        <source>Read only some of the Device&apos;s partitions: opens Custom Partitioning with the Device as the source, every partition ticked, and the Image File as where it goes. Untick what to leave out.</source>
        <translation>Leer solo algunas particiones del dispositivo: abre «Particionado personalizado» con el dispositivo como origen, todas las particiones marcadas y el archivo de imagen como destino. Desmarque lo que quiera dejar fuera.</translation>
    </message>
    <message>
        <source>Skip unpartitioned space on Read</source>
        <translation>Omitir espacio sin particionar al leer</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Check Device GPT</source>
        <translation>Comprobar la GPT del dispositivo</translation>
    </message>
    <message>
        <source>Custom Partitioning...</source>
        <translation>Particionado personalizado...</translation>
    </message>
    <message>
        <source>Put partitions from image files and disks onto a device, or into a new image file, in an order you choose, under a new partition table.</source>
        <translation>Poner particiones de archivos de imagen y discos en un dispositivo, o en un nuevo archivo de imagen, en el orden que elija, bajo una nueva tabla de particiones.</translation>
    </message>
    <message>
        <source>Image File Hash</source>
        <translation>Hash del archivo de imagen</translation>
    </message>
    <message>
        <source>Hash type to generate for image file</source>
        <translation>Tipo de hash que se generará para el archivo de imagen</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Generate selected hash on file</source>
        <translation>Generar hash seleccionado en el archivo</translation>
    </message>
    <message>
        <source>Generate</source>
        <translation>Generar</translation>
    </message>
    <message>
        <source>Copy hash to clipboard</source>
        <translation>Copiar hash en el portapapeles</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>Fix GPT after write</source>
        <translation>Corregir la GPT después de escribir</translation>
    </message>
    <message>
        <source>After writing, move the backup GPT to the end of the device and update the header to match, so Windows has nothing to &quot;repair&quot;. Leave unchecked to be warned to remove the device instead.</source>
        <translation>Después de escribir, mueve la GPT de respaldo al final del dispositivo y actualiza la cabecera para que coincida, de modo que Windows no tenga nada que «reparar». Si se deja sin marcar, se mostrará un aviso para retirar el dispositivo.</translation>
    </message>
    <message>
        <source>Show all devices</source>
        <translation>Mostrar todos los dispositivos</translation>
    </message>
    <message>
        <source>WinDiskImager</source>
        <translation>WinDiskImager</translation>
    </message>
    <message>
        <source>Also list fixed disks. Internal PCIe card readers often present the card as a non-removable device, which is otherwise hidden. The disk Windows is running from is never listed.</source>
        <translation>Muestra también los discos fijos. Los lectores de tarjetas PCIe internos suelen presentar la tarjeta como un dispositivo no extraíble, que de otro modo queda oculto. El disco desde el que se ejecuta Windows nunca aparece en la lista.</translation>
    </message>
    <message>
        <source>Reads the MBR or GPT of the Device and leaves out the unpartitioned space between and after its partitions. Everything before the first partition, where a bootloader is kept, is read as it is, and the first partition does not move. The backup GPT is moved to the new end of the image.</source>
        <translation type="vanished">Lee el MBR o la GPT del dispositivo y deja fuera el espacio sin particionar entre sus particiones y después de ellas. Todo lo que hay antes de la primera partición, donde se guarda el gestor de arranque, se lee tal cual, y la primera partición no se mueve. La GPT de respaldo se mueve al nuevo final de la imagen.</translation>
    </message>
    <message>
        <source>The compressed format to Read to: .img.zst is the fastest, .img.xz the smallest, and .img.gz the most widely supported</source>
        <translation>El formato comprimido de lectura: .img.zst es el más rápido, .img.xz el más pequeño y .img.gz el más compatible</translation>
    </message>
    <message>
        <source>Progress</source>
        <translation>Progreso</translation>
    </message>
    <message>
        <source>%p%</source>
        <translation>%p%</translation>
    </message>
    <message>
        <source>Cancel current process.</source>
        <translation>Cancelar el proceso actual.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Read data from &apos;Device&apos; to &apos;Image File&apos;</source>
        <translation>Leer datos del «Dispositivo» al «Archivo de imagen»</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Leer</translation>
    </message>
    <message>
        <source>Write data from &apos;Image File&apos; to &apos;Device&apos;</source>
        <translation>Escribir datos del «Archivo de imagen» al «Dispositivo»</translation>
    </message>
    <message>
        <source>Write</source>
        <translation>Escribir</translation>
    </message>
    <message>
        <source>Compare data in &apos;Device&apos; against &apos;Image File&apos;</source>
        <translation>Comparar los datos del «Dispositivo» con el «Archivo de imagen»</translation>
    </message>
    <message>
        <source>Verify the image file with the selected drive</source>
        <translation type="vanished">Comprobar el Archivo Imagen con la unidad seleccionada </translation>
    </message>
    <message>
        <source>Verify Only</source>
        <translation type="vanished">Solo Verificar</translation>
    </message>
    <message>
        <source>Exit Win32 Disk Imager</source>
        <translation type="vanished">Salir de Win32 Disk Imager</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation>Salir</translation>
    </message>
    <message>
        <source>Exit?</source>
        <translation>¿Salir?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt image file.
Are you sure you want to exit?</source>
        <translation>Salir ahora provocará un archivo de imagen dañado.
¿Está seguro de que quiere salir?</translation>
    </message>
    <message>
        <source>Exiting now will result in a corrupt disk.
Are you sure you want to exit?</source>
        <translation>Salir ahora provocará un disco dañado.
¿Está seguro de que quiere salir?</translation>
    </message>
    <message>
        <source>Select a disk image</source>
        <translation>Seleccione una imagen de disco</translation>
    </message>
    <message>
        <source>Generating...</source>
        <translation>Generando…</translation>
    </message>
    <message>
        <source>Cancel?</source>
        <translation>¿Cancelar?</translation>
    </message>
    <message>
        <source>Canceling now will result in a corrupt destination.
Are you sure you want to cancel?</source>
        <translation>Cancelar ahora provocará un destino dañado.
¿Está seguro de que quiere cancelar?</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Error de escritura</translation>
    </message>
    <message>
        <source>Image file cannot be located on the target device.</source>
        <translation>El archivo de imagen no puede estar en el dispositivo de destino.</translation>
    </message>
    <message>
        <source>Confirm overwrite</source>
        <translation>Confirmar sobrescritura</translation>
    </message>
    <message>
        <source>Waiting for a task.</source>
        <translation type="vanished">Esperando una tarea.</translation>
    </message>
    <message>
        <source>Exiting now will cancel verifying image.
Are you sure you want to exit?</source>
        <translation>Salir ahora cancelará la verificación de la imagen.
¿Está seguro de que quiere salir?</translation>
    </message>
    <message>
        <source>Cancel Verify.
Are you sure you want to cancel?</source>
        <translation>Cancelar la verificación.
¿Está seguro de que quiere cancelar?</translation>
    </message>
    <message>
        <source>Not enough available space!</source>
        <translation>¡No hay suficiente espacio disponible!</translation>
    </message>
    <message>
        <source>File Error</source>
        <translation>Error de archivo</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1
%2

%3

Physically remove the device NOW, before doing anything else, and do not re-insert it into this computer. Insert it into the target hardware instead.</source>
        <translation type="vanished">Escritura correcta, pero la tabla de particiones está en riesgo.

%1
%2

%3

Retire físicamente el dispositivo AHORA, antes de hacer cualquier otra cosa, y no vuelva a insertarlo en este equipo. Insértelo en el hardware de destino.</translation>
    </message>
    <message>
        <source>The selected file does not exist.</source>
        <translation>El archivo seleccionado no existe.</translation>
    </message>
    <message>
        <source>The specified file contains no data.</source>
        <translation>El archivo especificado no contiene datos.</translation>
    </message>
    <message>
        <source>Done.</source>
        <translation>Terminado.</translation>
    </message>
    <message>
        <source>Complete</source>
        <translation>Completo</translation>
    </message>
    <message>
        <source>Write Successful.</source>
        <translation>Escritura correcta.</translation>
    </message>
    <message>
        <source>Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</source>
        <translation>Imágenes de disco (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)</translation>
    </message>
    <message>
        <source>Compressed Disk Images (*.gz *.xz *.bz2 *.zst)</source>
        <translation>Imágenes de disco comprimidas (*.gz *.xz *.bz2 *.zst)</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Could not open the file to generate a checksum:
%1</source>
        <translation type="vanished">No se pudo abrir el archivo para generar la suma de comprobación:
%1</translation>
    </message>
    <message>
        <source>Please select a target device.</source>
        <translation>Seleccione un dispositivo de destino.</translation>
    </message>
    <message>
        <source>All files and data on this device will be deleted.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>Todos los archivos y datos de este dispositivo se eliminarán.
(Dispositivo de destino: %1)
¿Seguro que quiere continuar?</translation>
    </message>
    <message>
        <source>Device has mounted volumes</source>
        <translation>El dispositivo tiene volúmenes montados</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Everything on this device, on every one of its partitions, will be destroyed and cannot be recovered.

Check that %2 is not a drive you meant to keep.

Write to this device anyway?</source>
        <translation>%1 está montado en Windows como %2.

Todo lo que hay en este dispositivo, en todas y cada una de sus particiones, se destruirá y no podrá recuperarse.

Compruebe que %2 no es una unidad que quisiera conservar.

¿Escribir en este dispositivo de todos modos?</translation>
    </message>
    <message>
        <source>Write failed.</source>
        <translation>Escritura fallida.</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Error de dispositivo</translation>
    </message>
    <message>
        <source>The device reports a size of zero. If it is a card reader, the card may have been removed.</source>
        <translation>El dispositivo indica un tamaño de cero. Si es un lector de tarjetas, puede que se haya retirado la tarjeta.</translation>
    </message>
    <message>
        <source>Could not open the file to generate a hash:
%1</source>
        <translation>No se pudo abrir el archivo para calcular un hash:
%1</translation>
    </message>
    <message>
        <source>Hashing...</source>
        <translation>Calculando el hash…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a hash:
%1</source>
        <translation>No se pudo leer el archivo completo para calcular un hash:
%1</translation>
    </message>
    <message>
        <source>Hashing canceled.</source>
        <translation>Cálculo del hash cancelado.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Available: %2 sectors
  Sector Size: %3

The end of the image will not be written, so the device will not hold a complete image.

Continue Anyway?</source>
        <translation>El tamaño de la imagen supera al del dispositivo:
  Imagen: al menos %1 sectores
  Disponible: %2 sectores
  Tamaño de sector: %3

El final de la imagen no se escribirá, así que el dispositivo no contendrá una imagen completa.

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>Se necesita más espacio del disponible:
  Necesario: %1 sectores
  Disponible: %2 sectores
  Tamaño de sector: %3

No se pudo comprobar si el espacio sobrante contiene datos, porque la imagen está comprimida

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>Se necesita más espacio del disponible:
  Necesario: %1 sectores
  Disponible: %2 sectores
  Tamaño de sector: %3

El espacio sobrante SÍ parece contener datos

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>More space required than is available:
  Required: %1 sectors
  Available: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>Se necesita más espacio del disponible:
  Necesario: %1 sectores
  Disponible: %2 sectores
  Tamaño de sector: %3

El espacio sobrante no parece contener datos

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>Write cancelled.</source>
        <translation>Escritura cancelada.</translation>
    </message>
    <message>
        <source>Clearing old partition tables...</source>
        <translation>Borrando las tablas de particiones antiguas…</translation>
    </message>
    <message>
        <source>Could not clear the existing partition tables on the device.</source>
        <translation>No se pudieron borrar las tablas de particiones existentes en el dispositivo.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable image. Write the image again before using it.</source>
        <translation>El dispositivo se ha escrito parcialmente y ya no contiene una imagen utilizable. Vuelva a escribir la imagen antes de usarlo.</translation>
    </message>
    <message>
        <source>Fixing GPT...</source>
        <translation>Corrigiendo la GPT…</translation>
    </message>
    <message>
        <source>Image truncated</source>
        <translation>Imagen truncada</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">La imagen es mayor que el dispositivo, así que su parte final no se escribió y el dispositivo no contiene una imagen completa.

Esto solo se pudo detectar cuando el dispositivo se llenó, porque una imagen gzip no registra su tamaño sin comprimir.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT was made consistent with the device (%1), so Windows has no damaged table to repair. The device can be removed normally.</source>
        <translation type="vanished">Escritura correcta.

La GPT se ajustó al dispositivo (%1), de modo que Windows no tiene ninguna tabla dañada que reparar. El dispositivo puede retirarse con normalidad.</translation>
    </message>
    <message>
        <source>Write successful.

The image contains no GPT, so there is no partition table for Windows to repair. The device can be removed normally.</source>
        <translation type="vanished">Escritura correcta.

La imagen no contiene ninguna GPT, así que no hay tabla de particiones que Windows pueda reparar. El dispositivo puede retirarse con normalidad.</translation>
    </message>
    <message>
        <source>Write successful.</source>
        <translation>Escritura correcta.</translation>
    </message>
    <message>
        <source>Write Successful</source>
        <translation>Escritura correcta</translation>
    </message>
    <message>
        <source>The device has been taken offline and ejected.</source>
        <translation type="vanished">El dispositivo se ha puesto sin conexión y se ha expulsado.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline automatically.</source>
        <translation type="vanished">El dispositivo NO se pudo poner sin conexión automáticamente.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed automatically (%1).</source>
        <translation type="vanished">La GPT no se pudo corregir automáticamente (%1).</translation>
    </message>
    <message>
        <source>the GPT is malformed</source>
        <translation type="vanished">la GPT está mal formada</translation>
    </message>
    <message>
        <source>Fixing the GPT failed (%1).</source>
        <translation>No se pudo corregir la GPT (%1).</translation>
    </message>
    <message>
        <source>write error</source>
        <translation>error de escritura</translation>
    </message>
    <message>
        <source>The &quot;Fix GPT after write&quot; option is not enabled.</source>
        <translation type="vanished">La opción «Corregir la GPT después de escribir» no está activada.</translation>
    </message>
    <message>
        <source>This image IS affected by the Windows GPT rewrite bug.

It reserves space ahead of its first partition, so a rescan makes Windows rewrite the primary partition table to point at the wrong sectors. The result still passes Windows&apos; own checks, but Linux rejects it and the device will not boot.</source>
        <translation type="vanished">Esta imagen SÍ está afectada por el fallo de reescritura de la GPT de Windows.

Reserva espacio antes de su primera partición, así que un nuevo análisis hace que Windows reescriba la tabla de particiones primaria apuntando a sectores equivocados. El resultado sigue superando las comprobaciones del propio Windows, pero Linux lo rechaza y el dispositivo no arrancará.</translation>
    </message>
    <message>
        <source>This image is NOT affected by the Windows GPT rewrite bug.

Windows will still rewrite the table on a rescan, because the backup GPT is not at the end of the device, but for this layout the rewrite lands on the correct values. Removing the device now keeps it byte-identical to the image regardless.</source>
        <translation type="vanished">Esta imagen NO está afectada por el fallo de reescritura de la GPT de Windows.

Windows reescribirá igualmente la tabla en un nuevo análisis, porque la GPT de respaldo no está al final del dispositivo, pero con esta disposición la reescritura da con los valores correctos. Aun así, retirar ahora el dispositivo lo mantiene idéntico byte a byte a la imagen.</translation>
    </message>
    <message>
        <source>Whether this image is affected by the Windows GPT rewrite bug could not be determined. Assume it is: a rescan can leave the partition table rejected by Linux and the device unbootable.</source>
        <translation type="vanished">No se pudo determinar si esta imagen está afectada por el fallo de reescritura de la GPT de Windows. Dé por hecho que lo está: un nuevo análisis puede hacer que Linux rechace la tabla de particiones y que el dispositivo no arranque.</translation>
    </message>
    <message>
        <source>Remove the device now</source>
        <translation>Retire el dispositivo ahora</translation>
    </message>
    <message>
        <source>You do not have permission to read the selected file.</source>
        <translation>No tiene permiso para leer el archivo seleccionado.</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension.

Compressed images (.img.gz, .img.xz) can be written and verified.</source>
        <translation type="vanished">Las imágenes solo se pueden volver a leer sin comprimir. Elija un nombre de archivo sin la extensión .gz ni .xz.

Las imágenes comprimidas (.img.gz, .img.xz) se pueden escribir y verificar.</translation>
    </message>
    <message>
        <source>Read failed.</source>
        <translation>Lectura fallida.</translation>
    </message>
    <message>
        <source>Verify failed.</source>
        <translation>Verificación fallida.</translation>
    </message>
    <message>
        <source>The image is larger than the device:
  Image: at least %1 sectors
  Device: %2 sectors
  Sector Size: %3

Only the part that fits can be compared.

Continue Anyway?</source>
        <translation>El tamaño de la imagen supera al del dispositivo:
  Imagen: al menos %1 sectores
  Dispositivo: %2 sectores
  Tamaño de sector: %3

Solo se podrá comparar la parte que cabe.

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space could not be checked for data, because the image is compressed

Continue Anyway?</source>
        <translation>El tamaño de la imagen supera al del dispositivo:
  Imagen: %1 sectores
  Dispositivo: %2 sectores
  Tamaño de sector: %3

No se pudo comprobar si el espacio sobrante contiene datos, porque la imagen está comprimida

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>Verify cancelled.</source>
        <translation>Verificación cancelada.</translation>
    </message>
    <message>
        <source>Verifying...</source>
        <translation>Verificando…</translation>
    </message>
    <message>
        <source>Partition table damaged</source>
        <translation>Tabla de particiones dañada</translation>
    </message>
    <message>
        <source>Repair failed</source>
        <translation>Error en la reparación</translation>
    </message>
    <message>
        <source>The partition table could not be repaired: %1</source>
        <translation>No se pudo reparar la tabla de particiones: %1</translation>
    </message>
    <message>
        <source>Select partitions to include in the Image.</source>
        <translation type="vanished">Seleccione las particiones a incluir en la imagen.</translation>
    </message>
    <message>
        <source>Image larger than device</source>
        <translation>Imagen mayor que el dispositivo</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because a gzip image does not record its uncompressed size.</source>
        <translation type="vanished">La imagen es mayor que el dispositivo, así que solo se pudo comparar la parte que cabe. Todo lo comparado coincidió, pero el dispositivo no contiene una imagen completa.

Esto solo se pudo detectar al llegar al final del dispositivo, porque una imagen gzip no registra su tamaño sin comprimir.</translation>
    </message>
    <message>
        <source>[Disk %1]</source>
        <translation>[Disco %1]</translation>
    </message>
    <message>
        <source>Please specify an image file to use.</source>
        <translation>Especifique el archivo de imagen que quiere usar.</translation>
    </message>
    <message>
        <source>Scanning disks...</source>
        <translation>Analizando los discos…</translation>
    </message>
    <message>
        <source>Could not read the whole file to generate a checksum:
%1</source>
        <translation type="vanished">No se pudo leer el archivo completo para generar la suma de comprobación:
%1</translation>
    </message>
    <message>
        <source>Writing: %1 MB/s</source>
        <translation>Escribiendo: %1 MB/s</translation>
    </message>
    <message>
        <source>Reading: %1 MB/s</source>
        <translation>Leyendo: %1 MB/s</translation>
    </message>
    <message>
        <source>Verifying: %1 MB/s</source>
        <translation>Verificando: %1 MB/s</translation>
    </message>
    <message>
        <source>Hashing: %1 MB/s</source>
        <translation>Calculando el hash: %1 MB/s</translation>
    </message>
    <message>
        <source>Generating checksum...</source>
        <translation type="vanished">Generando la suma de comprobación…</translation>
    </message>
    <message>
        <source>Checksum canceled.</source>
        <translation type="vanished">Suma de comprobación cancelada.</translation>
    </message>
    <message>
        <source>%1 the primary GPT header points at sectors the partition entries are not in.

This is what Windows leaves behind when it rescans a card written without &quot;Fix GPT after write&quot;. No data has been lost, but the device will not boot and most tools will refuse the table.

Repair the partition table now?</source>
        <translation>%1 la cabecera GPT primaria apunta a sectores en los que no están las entradas de partición.

Esto es lo que deja Windows cuando reanaliza una tarjeta escrita sin «Corregir la GPT después de escribir». No se ha perdido ningún dato, pero el dispositivo no arrancará y la mayoría de las herramientas rechazarán la tabla.

¿Reparar la tabla de particiones ahora?</translation>
    </message>
    <message>
        <source>Please select a device.</source>
        <translation>Seleccione un dispositivo.</translation>
    </message>
    <message>
        <source>Could not lock the device.</source>
        <translation>No se pudo bloquear el dispositivo.</translation>
    </message>
    <message>
        <source>Could not open the device.</source>
        <translation>No se pudo abrir el dispositivo.</translation>
    </message>
    <message>
        <source>This device&apos;s partition table is broken:</source>
        <translation>La tabla de particiones de este dispositivo está dañada:</translation>
    </message>
    <message>
        <source>Partition table repaired.</source>
        <translation>Tabla de particiones reparada.</translation>
    </message>
    <message>
        <source>Partition table is still damaged.</source>
        <translation>La tabla de particiones sigue dañada.</translation>
    </message>
    <message>
        <source>Partition table is valid.</source>
        <translation>La tabla de particiones es válida.</translation>
    </message>
    <message>
        <source>Partition table</source>
        <translation>Tabla de particiones</translation>
    </message>
    <message>
        <source>The GPT on this device is valid: the header and the partition entries it points at agree.</source>
        <translation>La GPT de este dispositivo es válida: la cabecera y las entradas de partición a las que apunta coinciden.</translation>
    </message>
    <message>
        <source>No GPT on this device.</source>
        <translation>Este dispositivo no tiene GPT.</translation>
    </message>
    <message>
        <source>This device has no GPT, so it cannot have the damage this checks for.</source>
        <translation>Este dispositivo no tiene GPT, así que no puede presentar el daño que se comprueba aquí.</translation>
    </message>
    <message>
        <source>Could not read the partition table.</source>
        <translation>No se pudo leer la tabla de particiones.</translation>
    </message>
    <message>
        <source>The partition table could not be read, or is damaged in some way other than the one this repairs.</source>
        <translation>No se pudo leer la tabla de particiones, o está dañada de una forma distinta a la que esto repara.</translation>
    </message>
    <message>
        <source>The target device is also one of the sources.</source>
        <translation>El dispositivo de destino también es uno de los orígenes.</translation>
    </message>
    <message>
        <source>%1 is on the target device, and cannot be written to it.</source>
        <translation>%1 está en el dispositivo de destino y no se puede escribir en él.</translation>
    </message>
    <message>
        <source>Confirm write</source>
        <translation>Confirmar escritura</translation>
    </message>
    <message>
        <source>The device keeps the partitions ticked in the order, and they are not written to. Anything in its free space, and in the partitions taken out of its table, may be overwritten.
(Target Device: %1)
Are you sure you want to continue?</source>
        <translation>El dispositivo conserva las particiones marcadas en el orden, y no se escribe en ellas. Todo lo que haya en su espacio libre, y en las particiones sacadas de su tabla, puede sobrescribirse.
(Dispositivo de destino: %1)
¿Seguro que quiere continuar?</translation>
    </message>
    <message>
        <source>%1 is mounted in Windows as %2.

Its volumes are dismounted while it is written, and the device is ejected afterwards. The kept partitions are not changed.

Write to this device anyway?</source>
        <translation>%1 está montado en Windows como %2.

Sus volúmenes se desmontan mientras se escribe en él, y el dispositivo se expulsa después. Las particiones conservadas no se modifican.

¿Escribir en este dispositivo de todos modos?</translation>
    </message>
    <message>
        <source>The device list changed while you were confirming. Check the target device and try again.</source>
        <translation>La lista de dispositivos cambió mientras confirmaba. Compruebe el dispositivo de destino e inténtelo de nuevo.</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 termina en el sector %2, antes de que termine la partición que debe aportar ahí: la imagen está incompleta.</translation>
    </message>
    <message>
        <source>Sector %1 of the device does not match sector %2 of %3.</source>
        <translation>El sector %1 del dispositivo no coincide con el sector %2 de %3.</translation>
    </message>
    <message>
        <source>The device has been partially written and no longer holds a usable layout. Write it again before using it.</source>
        <translation>El dispositivo se ha escrito parcialmente y ya no contiene una disposición utilizable. Vuelva a escribirlo antes de usarlo.</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has changed since Custom Partitioning read it. Open Custom Partitioning again to plan from what the device holds now.</source>
        <translation>La tabla de particiones del dispositivo ha cambiado desde que «Particionado personalizado» la leyó. Vuelva a abrir «Particionado personalizado» para planificar a partir de lo que contiene ahora el dispositivo.</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Escribiendo…</translation>
    </message>
    <message>
        <source>The device&apos;s partition table has not been changed: it holds its partitions as before. Its free space may hold part of the new ones.</source>
        <translation>La tabla de particiones del dispositivo no se ha modificado: contiene sus particiones como antes. Su espacio libre puede contener parte de las nuevas.</translation>
    </message>
    <message>
        <source>The partition table on the device does not match what was written.</source>
        <translation>La tabla de particiones del dispositivo no coincide con lo que se escribió.</translation>
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
        <translation>La tabla de particiones %1 del dispositivo contiene ahora %2 particiones: %3 conservadas y %4 nuevas de %5 imágenes.</translation>
    </message>
    <message>
        <source>Write and verify successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Escritura y verificación correctas.

El dispositivo contiene una nueva tabla de particiones %1 con %2 particiones de %3 imágenes.</translation>
    </message>
    <message>
        <source>Write successful.

The device holds a new %1 partition table with %2 partitions from %3 images.</source>
        <translation>Escritura correcta.

El dispositivo contiene una nueva tabla de particiones %1 con %2 particiones de %3 imágenes.</translation>
    </message>
    <message>
        <source>Its backup is already at the end of the device, so Windows has nothing to repair.</source>
        <translation>Su copia de respaldo ya está al final del dispositivo, por lo que Windows no tiene nada que reparar.</translation>
    </message>
    <message>
        <source>Whether it boots depends on its bootloaders finding their partitions where they now are.</source>
        <translation>Que arranque depende de que sus gestores de arranque encuentren sus particiones donde están ahora.</translation>
    </message>
    <message>
        <source>Custom Partitioning</source>
        <translation>Particionado personalizado</translation>
    </message>
    <message>
        <source>The image is larger than the device, so the end of it was not written and the device does not hold a complete image.

This could only be detected once the device was full, because the compressed image does not record its uncompressed size.</source>
        <translation>La imagen es mayor que el dispositivo, así que su parte final no se escribió y el dispositivo no contiene una imagen completa.

Esto solo se pudo detectar cuando el dispositivo se llenó, porque la imagen comprimida no registra su tamaño sin comprimir.</translation>
    </message>
    <message>
        <source>Write successful.

The GPT now matches the device (%1), so Windows has nothing to repair. Remove the device normally.</source>
        <translation>Escritura correcta.

La GPT ahora coincide con el dispositivo (%1), por lo que Windows no tiene nada que reparar. Extraiga el dispositivo con normalidad.</translation>
    </message>
    <message>
        <source>Write successful.

This image uses an MBR partition table, not a GPT, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Escritura correcta.

Esta imagen usa una tabla de particiones MBR, no GPT, por lo que el error de reescritura de GPT de Windows no puede afectarla. Extraiga el dispositivo con normalidad.</translation>
    </message>
    <message>
        <source>Write successful.

This image has no partition table, so the Windows GPT rewrite bug cannot affect it. Remove the device normally.</source>
        <translation>Escritura correcta.

Esta imagen no tiene tabla de particiones, por lo que el error de reescritura de GPT de Windows no puede afectarla. Extraiga el dispositivo con normalidad.</translation>
    </message>
    <message>
        <source>The device is offline and ejected.</source>
        <translation>El dispositivo está sin conexión y expulsado.</translation>
    </message>
    <message>
        <source>The device could NOT be taken offline.</source>
        <translation>El dispositivo NO se pudo poner sin conexión.</translation>
    </message>
    <message>
        <source>The GPT could not be fixed (%1).</source>
        <translation>No se pudo corregir la GPT (%1).</translation>
    </message>
    <message>
        <source>malformed GPT</source>
        <translation>GPT con formato incorrecto</translation>
    </message>
    <message>
        <source>&quot;Fix GPT after write&quot; is off.</source>
        <translation>«Corregir la GPT después de escribir» está desactivado.</translation>
    </message>
    <message>
        <source>This image IS affected: it reserves space ahead of its first partition, so a rescan points the primary table at the wrong sectors. Windows still accepts the result; Linux does not, and the device will not boot.</source>
        <translation>Esta imagen SÍ está afectada: reserva espacio antes de su primera partición, por lo que un reanálisis hace que la tabla primaria apunte a sectores equivocados. Windows sigue aceptando el resultado; Linux no, y el dispositivo no arrancará.</translation>
    </message>
    <message>
        <source>This image is NOT affected: a rescan still rewrites the table, but for this layout it writes the correct values. Removing the device now keeps it identical to the image either way.</source>
        <translation>Esta imagen NO está afectada: un reanálisis sigue reescribiendo la tabla, pero con esta disposición escribe los valores correctos. Extraer el dispositivo ahora lo mantiene idéntico a la imagen en cualquier caso.</translation>
    </message>
    <message>
        <source>Whether this image is affected could not be determined. Assume it is: a rescan can leave a table that Linux rejects and the device will not boot.</source>
        <translation>No se pudo determinar si esta imagen está afectada. Supóngalo: un reanálisis puede dejar una tabla que Linux rechaza y el dispositivo no arrancará.</translation>
    </message>
    <message>
        <source>Write successful, but the partition table is at risk.

%1 %2

%3

Remove the device NOW and do not re-insert it here. Put it straight into the target hardware.</source>
        <translation>Escritura correcta, pero la tabla de particiones está en riesgo.

%1 %2

%3

Extraiga el dispositivo AHORA y no vuelva a insertarlo aquí. Colóquelo directamente en el hardware de destino.</translation>
    </message>
    <message>
        <source>The combined image ended early.</source>
        <translation>La imagen combinada terminó antes de tiempo.</translation>
    </message>
    <message>
        <source>Sector %1 of %2 is not what was written.</source>
        <translation>El sector %1 de %2 no es lo que se escribió.</translation>
    </message>
    <message>
        <source>%1 holds more than the combined image, or does not end cleanly.</source>
        <translation>%1 contiene más que la imagen combinada, o no termina limpiamente.</translation>
    </message>
    <message>
        <source>Write and verify successful.</source>
        <translation>Escritura y verificación correctas.</translation>
    </message>
    <message>
        <source>%1 holds a %2 partition table with %3 partitions from %4 images.</source>
        <translation>%1 contiene una tabla de particiones %2 con %3 particiones de %4 imágenes.</translation>
    </message>
    <message>
        <source>Its backup GPT ends the image; &quot;Fix GPT after write&quot; moves it to the end of a larger device when the image is written.</source>
        <translation>Su GPT de respaldo cierra la imagen; «Corregir la GPT después de escribir» la mueve al final de un dispositivo mayor cuando se escribe la imagen.</translation>
    </message>
    <message>
        <source>Choose Partitions</source>
        <translation type="vanished">Elegir particiones</translation>
    </message>
    <message>
        <source>Skipping unpartitioned space keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. An image of such a device read this way may not boot.</source>
        <translation type="vanished">Al omitir el espacio sin particionar se conservan solo las particiones y la tabla de particiones, además del espacio que una GPT reserve antes de sus particiones.

Algunas imágenes de arranque, como las de ordenadores de placa única, guardan datos del gestor de arranque fuera de las particiones. Una imagen de ese dispositivo leída así podría no arrancar.</translation>
    </message>
    <message>
        <source>Shrinking keeps only the partitions and the partition table, plus any space a GPT reserves ahead of its partitions.

Some bootable images, such as those for single-board computers, keep bootloader data outside the partitions. A shrunk image of such a device may not boot.</source>
        <translation type="vanished">Al reducir se conservan solo las particiones y la tabla de particiones, además del espacio que una GPT reserve antes de sus particiones.

Algunas imágenes de arranque, como las de ordenadores de placa única, guardan datos del gestor de arranque fuera de las particiones. Una imagen reducida de ese dispositivo podría no arrancar.</translation>
    </message>
    <message>
        <source>Choose which partitions to include in the image. Anything left unchecked is removed, the same as unpartitioned space.</source>
        <translation type="vanished">Elija qué particiones incluir en la imagen. Todo lo que quede sin marcar se elimina, igual que el espacio sin particionar.</translation>
    </message>
    <message>
        <source>Partition %1 -- %2</source>
        <translation type="vanished">Partición %1 -- %2</translation>
    </message>
    <message>
        <source>Partition %1 -- %2 -- %3</source>
        <translation type="vanished">Partición %1 -- %2 -- %3</translation>
    </message>
    <message>
        <source>At least one partition must stay checked.</source>
        <translation type="vanished">Al menos una partición debe permanecer marcada.</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Error de lectura</translation>
    </message>
    <message>
        <source>Images can only be read back uncompressed. Choose a file name without a .gz or .xz extension, or check &quot;Read to .img.gz&quot; or &quot;Read to .img.xz&quot;.</source>
        <translation type="vanished">Las imágenes solo se pueden leer sin comprimir. Elija un nombre de archivo sin la extensión .gz o .xz, o marque «Leer a .img.gz» o «Leer a .img.xz».</translation>
    </message>
    <message>
        <source>Please select a source device.</source>
        <translation>Seleccione un dispositivo de origen.</translation>
    </message>
    <message>
        <source>Confirm Overwrite</source>
        <translation>Confirmar sobrescritura</translation>
    </message>
    <message>
        <source>Are you sure you want to overwrite the specified file?</source>
        <translation>¿Está seguro de que quiere sobrescribir el archivo especificado?</translation>
    </message>
    <message>
        <source>No partition table was found on the device, so there is nothing to choose from. The whole device will be read.</source>
        <translation type="vanished">No se encontró ninguna tabla de particiones en el dispositivo, así que no hay nada para elegir. Se leerá el dispositivo completo.</translation>
    </message>
    <message>
        <source>Read canceled.</source>
        <translation type="vanished">Lectura cancelada.</translation>
    </message>
    <message>
        <source>Disk is not large enough for the specified image.</source>
        <translation>El disco no es lo suficientemente grande para la imagen especificada.</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Leyendo…</translation>
    </message>
    <message>
        <source>Read Canceled.</source>
        <translation>Lectura cancelada.</translation>
    </message>
    <message>
        <source>Read Successful.</source>
        <translation>Lectura correcta.</translation>
    </message>
    <message>
        <source>File Info</source>
        <translation>Información del archivo</translation>
    </message>
    <message>
        <source>Please specify a file to save data to.</source>
        <translation>Especifique el archivo en el que guardar los datos.</translation>
    </message>
    <message>
        <source>Verify Error</source>
        <translation>Error de verificación</translation>
    </message>
    <message>
        <source>Please select a device to verify against.</source>
        <translation>Seleccione un dispositivo con el que comparar.</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space DOES appear to contain data

Continue Anyway?</source>
        <translation>El tamaño de la imagen supera al del dispositivo:
  Imagen: %1 sectores
  Dispositivo: %2 sectores
  Tamaño de sector: %3

El espacio sobrante SÍ parece contener datos

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>Size of image larger than device:
  Image: %1 sectors
  Device: %2 sectors
  Sector Size: %3

The extra space does not appear to contain data

Continue Anyway?</source>
        <translation>El tamaño de la imagen supera al del dispositivo:
  Imagen: %1 sectores
  Dispositivo: %2 sectores
  Tamaño de sector: %3

El espacio sobrante no parece contener datos

¿Continuar de todos modos?</translation>
    </message>
    <message>
        <source>The device could not be read at sector %1.</source>
        <translation>No se pudo leer el dispositivo en el sector %1.</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is broken:</source>
        <translation>El dispositivo contiene la imagen correctamente, pero su tabla de particiones está dañada:</translation>
    </message>
    <message>
        <source>The device holds the image correctly, but its partition table is still broken. Write the image again with &quot;Fix GPT after write&quot; ticked, or run the verify again and accept the repair.</source>
        <translation>El dispositivo contiene la imagen correctamente, pero su tabla de particiones sigue dañada. Vuelva a escribir la imagen con «Corregir la GPT después de escribir» marcado, o ejecute de nuevo la verificación y acepte la reparación.</translation>
    </message>
    <message>
        <source>Verify Successful.

The device&apos;s partition table was damaged and has been repaired.</source>
        <translation>Verificación correcta.

La tabla de particiones del dispositivo estaba dañada y se ha reparado.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, and the GPT on the device is valid.</source>
        <translation>Verificación correcta.

La imagen y el dispositivo solo se diferencian en la GPT, y la GPT del dispositivo es válida.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT.</source>
        <translation>Verificación correcta.

La imagen y el dispositivo solo se diferencian en la GPT.</translation>
    </message>
    <message>
        <source>Size Mismatch!</source>
        <translation>¡Tamaño incorrecto!</translation>
    </message>
    <message>
        <source>Verify Failure</source>
        <translation>Error de verificación</translation>
    </message>
    <message>
        <source>Verification failed at sector: %1</source>
        <translation>La verificación falló en el sector: %1</translation>
    </message>
    <message>
        <source>The image is larger than the device, so only the part that fits could be compared. Everything compared matched, but the device does not hold a complete image.

This could only be detected at the end of the device, because the compressed image does not record its uncompressed size.</source>
        <translation>La imagen es mayor que el dispositivo, así que solo se pudo comparar la parte que cabe. Todo lo comparado coincidió, pero el dispositivo no contiene una imagen completa.

Esto solo se pudo detectar al llegar al final del dispositivo, porque la imagen comprimida no registra su tamaño sin comprimir.</translation>
    </message>
    <message>
        <source>Verify Successful.

The image and the device differ only in the GPT, which the &quot;Fix GPT after write&quot; option rewrites by design.</source>
        <translation type="vanished">Verificación correcta.

La imagen y el dispositivo solo se diferencian en la GPT, que la opción «Corregir la GPT después de escribir» reescribe intencionadamente.</translation>
    </message>
    <message>
        <source>

The device has been ejected. Remove it now.</source>
        <translation>

El dispositivo se ha expulsado. Retírelo ahora.</translation>
    </message>
    <message>
        <source>

The device could NOT be taken offline automatically.</source>
        <translation>

El dispositivo NO se pudo poner sin conexión automáticamente.</translation>
    </message>
    <message>
        <source>Verify Successful.</source>
        <translation>Verificación correcta.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>File Error</source>
        <translation>Error de archivo</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the file.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar obtener un identificador del archivo.
Error %1: %2</translation>
    </message>
    <message>
        <source>Device Error</source>
        <translation>Error de dispositivo</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get a handle on the device.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar obtener un identificador del dispositivo.
Error %1: %2</translation>
    </message>
    <message>
        <source>Failed to get the free space on the volume holding %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation>Error al obtener el espacio libre en el volumen que contiene %1.
Error %2: %3
Se omitirá la comprobación del espacio libre.</translation>
    </message>
    <message>
        <source>Lock Error</source>
        <translation>Error de bloqueo</translation>
    </message>
    <message>
        <source>An error occurred when attempting to lock the volume.
Error %1: %2</source>
        <translation type="vanished">Se ha producido un error al intentar bloquear el volumen.
Error %1: %2</translation>
    </message>
    <message>
        <source>Unlock Error</source>
        <translation>Error de desbloqueo</translation>
    </message>
    <message>
        <source>An error occurred when attempting to unlock the volume.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar desbloquear el volumen.
Error %1: %2</translation>
    </message>
    <message>
        <source>Dismount Error</source>
        <translation>Error de desmontaje</translation>
    </message>
    <message>
        <source>An error occurred when attempting to dismount the volume.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar desmontar el volumen.
Error %1: %2</translation>
    </message>
    <message>
        <source>Read Error</source>
        <translation>Error de lectura</translation>
    </message>
    <message>
        <source>Sector count too large.</source>
        <translation>El número de sectores es demasiado grande.</translation>
    </message>
    <message>
        <source>Unable to allocate memory for read buffer.</source>
        <translation>No se pudo reservar memoria para el búfer de lectura.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to read data from handle.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar leer datos del identificador.
Error %1: %2</translation>
    </message>
    <message>
        <source>Write Error</source>
        <translation>Error de escritura</translation>
    </message>
    <message>
        <source>An error occurred when attempting to write data to handle.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar escribir datos en el identificador.
Error %1: %2</translation>
    </message>
    <message>
        <source>The device took only %1 of %2 bytes. The image on the device is incomplete.</source>
        <translation>El dispositivo solo admitió %1 de %2 bytes. La imagen del dispositivo está incompleta.</translation>
    </message>
    <message>
        <source>An error occurred when attempting to get the device&apos;s geometry.
Error %1: %2</source>
        <translation>Se ha producido un error al intentar obtener la geometría del dispositivo.
Error %1: %2</translation>
    </message>
    <message>
        <source>An error occurred while getting the file size.
Error %1: %2</source>
        <translation>Se ha producido un error al obtener el tamaño del archivo.
Error %1: %2</translation>
    </message>
    <message>
        <source>Free Space Error</source>
        <translation>Error de espacio libre</translation>
    </message>
    <message>
        <source>Failed to get the free space on drive %1.
Error %2: %3
Checking of free space will be skipped.</source>
        <translation type="vanished">Error al obtener el espacio libre en la unidad %1.
Error %2:%3
La comprobación del espacio libre será omitida.</translation>
    </message>
    <message>
        <source>Unknown device</source>
        <translation>Dispositivo desconocido</translation>
    </message>
    <message>
        <source>Could not list the volumes on this computer.
Error %1</source>
        <translation>No se pudieron enumerar los volúmenes de este equipo.
Error %1</translation>
    </message>
    <message>
        <source>Could not lock volume %1: it is still in use.
Close any program using the device and try again.
Error %2</source>
        <translation>No se pudo bloquear el volumen %1: sigue en uso.
Cierre cualquier programa que esté usando el dispositivo e inténtelo de nuevo.
Error %2</translation>
    </message>
    <message>
        <source>the primary GPT header size is out of range</source>
        <translation>el tamaño de la cabecera GPT primaria está fuera de rango</translation>
    </message>
    <message>
        <source>the primary GPT header checksum is invalid</source>
        <translation>la suma de comprobación de la cabecera GPT primaria no es válida</translation>
    </message>
    <message>
        <source>%1, no partition table</source>
        <translation>%1, sin tabla de particiones</translation>
    </message>
    <message>
        <source>unrecognized filesystem, no partition table</source>
        <translation>sistema de archivos no reconocido, sin tabla de particiones</translation>
    </message>
    <message>
        <source>the GPT header size is out of range</source>
        <translation>el tamaño de la cabecera GPT está fuera de rango</translation>
    </message>
    <message>
        <source>the GPT header checksum is invalid</source>
        <translation>la suma de comprobación de la cabecera GPT no es válida</translation>
    </message>
    <message>
        <source>the GPT partition entry array is not where the header says</source>
        <translation>la matriz de entradas de partición GPT no está donde indica la cabecera</translation>
    </message>
    <message>
        <source>FirstUsableLBA lies inside the partition table</source>
        <translation>FirstUsableLBA está dentro de la tabla de particiones</translation>
    </message>
    <message>
        <source>partition %1 runs past the end of the image</source>
        <translation>la partición %1 sobrepasa el final de la imagen</translation>
    </message>
    <message>
        <source>partition %1 describes an impossible range</source>
        <translation>la partición %1 describe un intervalo imposible</translation>
    </message>
    <message>
        <source>the GPT holds no partitions</source>
        <translation>la GPT no contiene particiones</translation>
    </message>
    <message>
        <source>two partitions overlap</source>
        <translation>dos particiones se solapan</translation>
    </message>
    <message>
        <source>extended, with its logical partitions (0x%1)</source>
        <translation>extendida, con sus particiones lógicas (0x%1)</translation>
    </message>
    <message>
        <source>type 0x%1</source>
        <translation>tipo 0x%1</translation>
    </message>
    <message>
        <source>the MBR holds no partitions</source>
        <translation>el MBR no contiene particiones</translation>
    </message>
    <message>
        <source>the MBR has more than one extended partition</source>
        <translation>el MBR tiene más de una partición extendida</translation>
    </message>
    <message>
        <source>the image is smaller than one sector</source>
        <translation>la imagen es más pequeña que un sector</translation>
    </message>
    <message>
        <source>the image has a protective MBR but no GPT header</source>
        <translation>la imagen tiene un MBR de protección pero no una cabecera GPT</translation>
    </message>
    <message>
        <source>an extended MBR partition cannot go on a GPT: choose the logical partitions&apos; image as the lead-in, or leave it out</source>
        <translation>una partición MBR extendida no puede ir en una GPT: elija la imagen de las particiones lógicas como zona de arranque, o déjela fuera</translation>
    </message>
    <message>
        <source>MBR partition type 0x%1 has no GPT equivalent this program knows</source>
        <translation>el tipo de partición MBR 0x%1 no tiene un equivalente GPT que este programa conozca</translation>
    </message>
    <message>
        <source>GPT partition type %1 has no MBR equivalent</source>
        <translation>el tipo de partición GPT %1 no tiene equivalente MBR</translation>
    </message>
    <message>
        <source>the device has no partition table to keep</source>
        <translation>el dispositivo no tiene ninguna tabla de particiones que conservar</translation>
    </message>
    <message>
        <source>a lead-in cannot be used while the device keeps its own partitions</source>
        <translation>no se puede usar una zona de arranque mientras el dispositivo conserva sus propias particiones</translation>
    </message>
    <message>
        <source>only a device can keep its own partitions</source>
        <translation>solo un dispositivo puede conservar sus propias particiones</translation>
    </message>
    <message>
        <source>a free space has no size</source>
        <translation>un espacio libre no tiene tamaño</translation>
    </message>
    <message>
        <source>the device&apos;s own partitions must stay in the order they are on it</source>
        <translation>las particiones propias del dispositivo deben quedarse en el orden en que están en él</translation>
    </message>
    <message>
        <source>no partitions are chosen</source>
        <translation>no se ha elegido ninguna partición</translation>
    </message>
    <message>
        <source>a chosen partition does not exist</source>
        <translation>una partición elegida no existe</translation>
    </message>
    <message>
        <source>a partition is chosen twice</source>
        <translation>una partición se ha elegido dos veces</translation>
    </message>
    <message>
        <source>the size of an image with no partition table is not known: scan it first</source>
        <translation>no se conoce el tamaño de una imagen sin tabla de particiones: analícela primero</translation>
    </message>
    <message>
        <source>the lead-in image has no partition table</source>
        <translation>la imagen de la zona de arranque no tiene tabla de particiones</translation>
    </message>
    <message>
        <source>an MBR holds at most four partitions, and %1 are chosen</source>
        <translation>un MBR contiene como máximo cuatro particiones, y se han elegido %1</translation>
    </message>
    <message>
        <source>an MBR can hold only one extended partition</source>
        <translation>un MBR solo puede contener una partición extendida</translation>
    </message>
    <message>
        <source>the GPT has room for %1 partitions, and %2 are chosen</source>
        <translation>la GPT tiene espacio para %1 particiones, y se han elegido %2</translation>
    </message>
    <message>
        <source>there is %1 MB too little free space before the device&apos;s partition %2, which is kept</source>
        <translation>faltan %1 MB de espacio libre antes de la partición %2 del dispositivo, que se conserva</translation>
    </message>
    <message>
        <source>the layout no longer fits a 32-bit MBR entry</source>
        <translation>la disposición ya no cabe en una entrada MBR de 32 bits</translation>
    </message>
    <message>
        <source>the partitions need %1 MB and the device has %2 MB</source>
        <translation>las particiones necesitan %1 MB y el dispositivo tiene %2 MB</translation>
    </message>
    <message>
        <source>the GPT entry array does not fit on the device</source>
        <translation>la matriz de entradas GPT no cabe en el dispositivo</translation>
    </message>
    <message>
        <source>the GPT partition entry array checksum is invalid</source>
        <translation>la suma de comprobación de la matriz de entradas de partición GPT no es válida</translation>
    </message>
    <message>
        <source>a partition extends past the end of the device</source>
        <translation>una partición se extiende más allá del final del dispositivo</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2; the stale copy at LBA %3 was cleared</source>
        <translation>GPT de respaldo movida al LBA %1; el último LBA utilizable es ahora %2; la copia obsoleta en el LBA %3 se borró</translation>
    </message>
    <message>
        <source>backup GPT moved to LBA %1; last usable LBA is now %2</source>
        <translation>GPT de respaldo movida al LBA %1; el último LBA utilizable es ahora %2</translation>
    </message>
    <message>
        <source>the device has a GPT, which its MBR only mirrors</source>
        <translation>el dispositivo tiene una GPT, que su MBR solo refleja</translation>
    </message>
    <message>
        <source>the MBR holds no partitions to shrink to</source>
        <translation>el MBR no contiene particiones a las que reducir</translation>
    </message>
    <message>
        <source>the repacked layout no longer fits a 32-bit MBR entry</source>
        <translation>el diseño reempaquetado ya no cabe en una entrada MBR de 32 bits</translation>
    </message>
    <message>
        <source>the device is already this tight; nothing to shrink</source>
        <translation>el dispositivo ya está así de ajustado; no hay nada que reducir</translation>
    </message>
    <message>
        <source>FirstUsableLBA is not usable for repacking</source>
        <translation>FirstUsableLBA no es válido para reempaquetar</translation>
    </message>
    <message>
        <source>the GPT holds no partitions to shrink to</source>
        <translation>la GPT no contiene particiones a las que reducir</translation>
    </message>
    <message>
        <source>a partition entry describes an impossible range</source>
        <translation>una entrada de partición describe un intervalo imposible</translation>
    </message>
    <message>
        <source>the device geometry is not usable</source>
        <translation>la geometría del dispositivo no es utilizable</translation>
    </message>
    <message>
        <source>the primary GPT header is not readable</source>
        <translation>no se puede leer la cabecera GPT primaria</translation>
    </message>
    <message>
        <source>the GPT entry array geometry is not usable</source>
        <translation>la geometría de la matriz de entradas GPT no es utilizable</translation>
    </message>
    <message>
        <source>the device is too small to hold an entry array</source>
        <translation>el dispositivo es demasiado pequeño para contener una matriz de entradas</translation>
    </message>
    <message>
        <source>the partition entries could not be read</source>
        <translation>no se pudieron leer las entradas de partición</translation>
    </message>
    <message>
        <source>the partition entries are not at LBA 2, so this is not the damage this can repair</source>
        <translation>las entradas de partición no están en el LBA 2, así que este no es el daño que esto puede reparar</translation>
    </message>
    <message>
        <source>the repaired header could not be written</source>
        <translation>no se pudo escribir la cabecera reparada</translation>
    </message>
    <message>
        <source>PartitionEntryLBA pointed back at LBA 2 and the header checksum rebuilt</source>
        <translation>PartitionEntryLBA se ha vuelto a apuntar al LBA 2 y se ha recalculado la suma de comprobación de la cabecera</translation>
    </message>
    <message>
        <source>The device reports a sector size of zero.</source>
        <translation>El dispositivo indica un tamaño de sector de cero.</translation>
    </message>
    <message>
        <source>Disk %1 could not be opened (error %2).</source>
        <translation>No se pudo abrir el disco %1 (error %2).</translation>
    </message>
    <message>
        <source>The size of disk %1 could not be read (error %2).</source>
        <translation>No se pudo leer el tamaño del disco %1 (error %2).</translation>
    </message>
    <message>
        <source>Disk %1 has %2-byte sectors, not %3.</source>
        <translation>El disco %1 tiene sectores de %2 bytes, no de %3.</translation>
    </message>
    <message>
        <source>The image file could not be opened (error %1).</source>
        <translation>No se pudo abrir el archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The size of the image file could not be read (error %1).</source>
        <translation>No se pudo leer el tamaño del archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The image file could not be read (error %1).</source>
        <translation>No se pudo leer el archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The image file could not be rewound (error %1).</source>
        <translation>No se pudo rebobinar el archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The bzip2 decompressor could not be started (bzip2 error %1).</source>
        <translation>No se pudo iniciar el descompresor bzip2 (error de bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd decompressor could not be started (zstd error %1).</source>
        <translation>No se pudo iniciar el descompresor zstd (error de zstd %1).</translation>
    </message>
    <message>
        <source>The gzip decompressor could not be started (zlib error %1).</source>
        <translation>No se pudo iniciar el descompresor gzip (error de zlib %1).</translation>
    </message>
    <message>
        <source>The xz decompressor could not be started (lzma error %1).</source>
        <translation>No se pudo iniciar el descompresor xz (error de lzma %1).</translation>
    </message>
    <message>
        <source>The image file ends in the middle of the compressed data. It is truncated or damaged.</source>
        <translation>El archivo de imagen termina en mitad de los datos comprimidos. Está truncado o dañado.</translation>
    </message>
    <message>
        <source>The gzip image could not be decompressed.</source>
        <translation>No se pudo descomprimir la imagen gzip.</translation>
    </message>
    <message>
        <source>The gzip image is damaged (zlib error %1).</source>
        <translation>La imagen gzip está dañada (error de zlib %1).</translation>
    </message>
    <message>
        <source>The bzip2 image could not be decompressed.</source>
        <translation>No se pudo descomprimir la imagen bzip2.</translation>
    </message>
    <message>
        <source>The bzip2 image is damaged (bzip2 error %1).</source>
        <translation>La imagen bzip2 está dañada (error de bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd image is damaged (zstd error %1).</source>
        <translation>La imagen zstd está dañada (error de zstd %1).</translation>
    </message>
    <message>
        <source>The xz image is damaged (lzma error %1).</source>
        <translation>La imagen xz está dañada (error de lzma %1).</translation>
    </message>
    <message>
        <source>A compressed image can only be read forwards.</source>
        <translation>Una imagen comprimida solo se puede leer hacia delante.</translation>
    </message>
    <message>
        <source>The image file could not be created (error %1).</source>
        <translation>No se pudo crear el archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The gzip compressor could not be started (zlib error %1).</source>
        <translation>No se pudo iniciar el compresor gzip (error de zlib %1).</translation>
    </message>
    <message>
        <source>The xz compressor could not be started (lzma error %1).</source>
        <translation>No se pudo iniciar el compresor xz (error de lzma %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor could not be started (bzip2 error %1).</source>
        <translation>No se pudo iniciar el compresor bzip2 (error de bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor could not be started (zstd error %1).</source>
        <translation>No se pudo iniciar el compresor zstd (error de zstd %1).</translation>
    </message>
    <message>
        <source>The gzip compressor failed (zlib error %1).</source>
        <translation>El compresor gzip falló (error de zlib %1).</translation>
    </message>
    <message>
        <source>The xz compressor failed (lzma error %1).</source>
        <translation>El compresor xz falló (error de lzma %1).</translation>
    </message>
    <message>
        <source>The bzip2 compressor failed (bzip2 error %1).</source>
        <translation>El compresor bzip2 falló (error de bzip2 %1).</translation>
    </message>
    <message>
        <source>The zstd compressor failed (zstd error %1).</source>
        <translation>El compresor zstd falló (error de zstd %1).</translation>
    </message>
    <message>
        <source>The image file could not be written (error %1).</source>
        <translation>No se pudo escribir el archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>The image file is not open for writing.</source>
        <translation>El archivo de imagen no está abierto para escritura.</translation>
    </message>
    <message>
        <source>The image file could not be flushed (error %1).</source>
        <translation>No se pudo vaciar el búfer del archivo de imagen (error %1).</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>%1 ends at sector %2, before the partition it is to supply there does: the image is incomplete.</source>
        <translation>%1 termina en el sector %2, antes de que termine la partición que debe aportar ahí: la imagen está incompleta.</translation>
    </message>
    <message>
        <source>Disk %1 (%2)</source>
        <translation>Disco %1 (%2)</translation>
    </message>
    <message>
        <source>Source disks will be dismounted</source>
        <translation>Se desmontarán los discos de origen</translation>
    </message>
    <message>
        <source>While they are read, the volumes on these source disks are locked and dismounted, so nothing changes them half way through:

%1

Programs using them lose them until the run ends. Nothing on them is changed. Continue?</source>
        <translation>Mientras se leen, los volúmenes de estos discos de origen se bloquean y se desmontan, para que nada los modifique a mitad del proceso:

%1

Los programas que los usan los pierden hasta que termine la operación. No se modifica nada en ellos. ¿Continuar?</translation>
    </message>
</context>
</TS>
