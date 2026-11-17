# La Pulpe — Proyecto final Programación I

Universidad Nacional, Sede Regional Chorotega, Campus Nicoya. Curso EIF201, II ciclo 2026.

## Descripción
Aplicación de escritorio para administrar productos e inventario, ventas en efectivo, historial y comprobantes, usuarios con roles y registro de actividades. Desarrollada en C++ con Qt Widgets y Visual Studio.

## Integrantes
- Víctor Emel Díaz Hernández — usuarios, autenticación, permisos e integración; Scrum Master y responsable de entrega.
- José Julián Rivera Brais — productos e inventario.
- Josué David Jiménez Seas — ventas y cobros.
- Samuel Moisés Gómez Ruiz — historial y tickets.

## Requisitos
- Windows y Visual Studio con herramientas de C++ para escritorio.
- Qt y extensión Qt Visual Studio Tools, configuradas para la versión de Qt usada por el proyecto.
- Dependencias incluidas o referenciadas por el código del repositorio.

## Compilación y ejecución
1. Clonar o descargar el repositorio y extraerlo sin alterar la estructura de carpetas.
2. Abrir el archivo de solución de Visual Studio (`.sln` o `.slnx`, según la versión).
3. Comprobar que Visual Studio reconozca la versión de Qt y que la configuración del proyecto apunte a ella.
4. Seleccionar la configuración de compilación adecuada (por ejemplo, x64 Debug).
5. Compilar la solución y corregir cualquier ruta local de Qt o dependencia que no exista en el equipo de destino.
6. Ejecutar desde Visual Studio. Para ejecutar fuera del IDE, asegurar la disponibilidad de las bibliotecas Qt necesarias.

## Funcionalidades
- Administrador: gestión de productos, inventario, usuarios, ventas, historial y registro de actividades.
- Cajero: operaciones de ventas e historial permitidas por el sistema.
- Venta: carrito, validación de existencias, cobro en efectivo, cálculo de cambio y generación de tickets TXT/HTML.
- Persistencia: productos en `productos.txt`, ventas en `ventas.json`, usuarios en `usuarios.json` y actividades en archivos JSON diarios en `RegistroActividades`.

## Pruebas funcionales realizadas
Se verificaron inicio de sesión y restricciones por rol, venta completa, rechazo por pago insuficiente, rechazo por inventario insuficiente, rechazo de código de producto duplicado, recuperación de historial tras reiniciar y generación de tickets TXT y HTML. Ver informe final para evidencias y responsables.

## Versión
**v1.0** — versión de entrega. Confirmar que la etiqueta o release se haya creado en GitHub tras aprobar el pull request de `development` a `main`.

## Observaciones
El prototipo HTML es una demostración independiente; la aplicación entregable es la implementación C++/Qt. No se deben incluir credenciales reales en el repositorio. La compilación en otro equipo puede requerir ajustar la instalación y rutas de Qt.
