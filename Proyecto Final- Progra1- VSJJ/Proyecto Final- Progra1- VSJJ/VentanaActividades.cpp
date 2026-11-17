
#include "VentanaActividades.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QFrame>
#include <QDate>
#include <QSignalBlocker>
#include <QStringList>

#include <algorithm>
#include <set>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

VentanaActividades::VentanaActividades(
    GestorActividades* gestorActividades,
    QWidget* parent
)
    : QDialog(parent),
      gestorActividades(gestorActividades),
      tablaActividades(nullptr),
      selectorFecha(nullptr),
      txtBuscarUsuario(nullptr),
      cmbTipoActividad(nullptr),
      btnActualizar(nullptr),
      btnHoy(nullptr),
      btnCerrar(nullptr),
      lblTotalActividades(nullptr),
      lblEstado(nullptr)
{
    configurarInterfaz();
    configurarEstilos();
    configurarTabla();

    // ======================================
    // CONECTAR BOTON ACTUALIZAR
    // ======================================

    connect(
        btnActualizar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            cargarActividades();
        }
    );

    // ======================================
    // CONECTAR BOTON HOY
    // ======================================

    connect(
        btnHoy,
        &QPushButton::clicked,
        this,
        [this]()
        {
            selectorFecha->setDate(
                QDate::currentDate()
            );

            cargarActividades();
        }
    );

    // ======================================
    // CONECTAR BOTON CERRAR
    // ======================================

    connect(
        btnCerrar,
        &QPushButton::clicked,
        this,
        &QDialog::accept
    );

    // ======================================
    // FILTRO POR FECHA
    // ======================================

    connect(
        selectorFecha,
        &QDateEdit::dateChanged,
        this,
        [this](const QDate&)
        {
            cargarActividades();
        }
    );

    // ======================================
    // FILTRO POR USUARIO
    // ======================================

    connect(
        txtBuscarUsuario,
        &QLineEdit::textChanged,
        this,
        [this](const QString&)
        {
            aplicarFiltros();
        }
    );

    // ======================================
    // FILTRO POR TIPO
    // ======================================

    connect(
        cmbTipoActividad,
        QOverload<int>::of(
            &QComboBox::currentIndexChanged
        ),
        this,
        [this](int)
        {
            aplicarFiltros();
        }
    );

    // ======================================
    // CARGAR ACTIVIDADES INICIALES
    // ======================================

    cargarActividades();
}

// ==========================================
// DESTRUCTOR
// ==========================================

VentanaActividades::~VentanaActividades()
{
    // Qt libera automaticamente los widgets
    // que pertenecen a esta ventana.
}

// ==========================================
// CONFIGURAR INTERFAZ
// ==========================================

void VentanaActividades::configurarInterfaz()
{
    setWindowTitle(
        "La Pulpe - Registro de Actividades"
    );

    setMinimumSize(1050, 650);
    resize(1200, 720);

    // ======================================
    // DISTRIBUCION PRINCIPAL
    // ======================================

    QVBoxLayout* layoutPrincipal =
        new QVBoxLayout(this);

    layoutPrincipal->setContentsMargins(
        25, 25, 25, 25
    );

    layoutPrincipal->setSpacing(18);

    // ======================================
    // TITULO PRINCIPAL
    // ======================================

    QLabel* lblTitulo = new QLabel(
        "REGISTRO DE ACTIVIDADES",
        this
    );

    lblTitulo->setObjectName(
        "tituloActividades"
    );

    QLabel* lblSubtitulo = new QLabel(
        "Consulta y seguimiento de operaciones "
        "realizadas en La Pulpe",
        this
    );

    lblSubtitulo->setObjectName(
        "subtituloActividades"
    );

    layoutPrincipal->addWidget(lblTitulo);
    layoutPrincipal->addWidget(lblSubtitulo);

    // ======================================
    // CONTENEDOR DE FILTROS
    // ======================================

    QFrame* panelFiltros = new QFrame(this);

    panelFiltros->setObjectName(
        "panelFiltros"
    );

    QGridLayout* layoutFiltros =
        new QGridLayout(panelFiltros);

    layoutFiltros->setContentsMargins(
        18, 18, 18, 18
    );

    layoutFiltros->setHorizontalSpacing(15);
    layoutFiltros->setVerticalSpacing(10);

    // ======================================
    // SELECTOR DE FECHA
    // ======================================

    QLabel* lblFecha = new QLabel(
        "Fecha:",
        panelFiltros
    );

    selectorFecha = new QDateEdit(
        QDate::currentDate(),
        panelFiltros
    );

    selectorFecha->setCalendarPopup(true);
    selectorFecha->setDisplayFormat(
        "dd/MM/yyyy"
    );

    selectorFecha->setMaximumDate(
        QDate::currentDate()
    );

    // ======================================
    // BUSQUEDA POR USUARIO
    // ======================================

    QLabel* lblUsuario = new QLabel(
        "Buscar usuario:",
        panelFiltros
    );

    txtBuscarUsuario = new QLineEdit(
        panelFiltros
    );

    txtBuscarUsuario->setPlaceholderText(
        "Escriba el nombre del usuario..."
    );

    txtBuscarUsuario->setClearButtonEnabled(
        true
    );

    // ======================================
    // FILTRO POR ACTIVIDAD
    // ======================================

    QLabel* lblTipo = new QLabel(
        "Tipo de actividad:",
        panelFiltros
    );

    cmbTipoActividad = new QComboBox(
        panelFiltros
    );

    cmbTipoActividad->addItem(
        "Todas las actividades"
    );

    // ======================================
    // AGREGAR FILTROS
    // ======================================

    layoutFiltros->addWidget(
        lblFecha, 0, 0
    );

    layoutFiltros->addWidget(
        lblUsuario, 0, 1
    );

    layoutFiltros->addWidget(
        lblTipo, 0, 2
    );

    layoutFiltros->addWidget(
        selectorFecha, 1, 0
    );

    layoutFiltros->addWidget(
        txtBuscarUsuario, 1, 1
    );

    layoutFiltros->addWidget(
        cmbTipoActividad, 1, 2
    );

    layoutFiltros->setColumnStretch(0, 1);
    layoutFiltros->setColumnStretch(1, 2);
    layoutFiltros->setColumnStretch(2, 2);

    layoutPrincipal->addWidget(
        panelFiltros
    );

    // ======================================
    // BOTONES DE CONSULTA
    // ======================================

    QHBoxLayout* layoutBotones =
        new QHBoxLayout();

    btnActualizar = new QPushButton(
        "Actualizar registros",
        this
    );

    btnActualizar->setObjectName(
        "btnActualizarActividades"
    );

    btnHoy = new QPushButton(
        "Ir a hoy",
        this
    );

    btnHoy->setObjectName(
        "btnHoyActividades"
    );

    layoutBotones->addWidget(
        btnActualizar
    );

    layoutBotones->addWidget(
        btnHoy
    );

    layoutBotones->addStretch();

    layoutPrincipal->addLayout(
        layoutBotones
    );

    // ======================================
    // TABLA DE ACTIVIDADES
    // ======================================

    tablaActividades = new QTableWidget(
        this
    );

    tablaActividades->setObjectName(
        "tablaActividades"
    );

    layoutPrincipal->addWidget(
        tablaActividades,
        1
    );

    // ======================================
    // INFORMACION INFERIOR
    // ======================================

    QHBoxLayout* layoutInferior =
        new QHBoxLayout();

    lblTotalActividades = new QLabel(
        "Actividades mostradas: 0",
        this
    );

    lblTotalActividades->setObjectName(
        "lblTotalActividades"
    );

    lblEstado = new QLabel(
        "Listo para consultar registros",
        this
    );

    lblEstado->setObjectName(
        "lblEstadoActividades"
    );

    btnCerrar = new QPushButton(
        "Cerrar",
        this
    );

    btnCerrar->setObjectName(
        "btnCerrarActividades"
    );

    layoutInferior->addWidget(
        lblTotalActividades
    );

    layoutInferior->addStretch();

    layoutInferior->addWidget(
        lblEstado
    );

    layoutInferior->addSpacing(20);

    layoutInferior->addWidget(
        btnCerrar
    );

    layoutPrincipal->addLayout(
        layoutInferior
    );
}

// ==========================================
// CONFIGURAR ESTILOS
// ==========================================

void VentanaActividades::configurarEstilos()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #F4F7F4;
            color: #26382B;
            font-family: "Segoe UI";
            font-size: 13px;
        }

        QLabel#tituloActividades {
            font-size: 25px;
            font-weight: bold;
            color: #174D32;
        }

        QLabel#subtituloActividades {
            font-size: 13px;
            color: #63736A;
        }

        QFrame#panelFiltros {
            background-color: white;
            border: 1px solid #DDE7DE;
            border-radius: 12px;
        }

        QLabel {
            color: #26382B;
        }

        QDateEdit,
        QLineEdit,
        QComboBox {
            background-color: white;
            color: #26382B;
            border: 1px solid #C9D8CD;
            border-radius: 7px;
            padding: 8px;
            min-height: 22px;
        }

        QDateEdit:focus,
        QLineEdit:focus,
        QComboBox:focus {
            border: 2px solid #25834F;
        }

        QComboBox QAbstractItemView {
            background-color: white;
            color: #26382B;
            selection-background-color: #DCEFE2;
            selection-color: #174D32;
        }

        QPushButton {
            background-color: #E6EEE8;
            color: #174D32;
            border: 1px solid #C9D8CD;
            border-radius: 8px;
            padding: 10px 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #D4E6D9;
        }

        QPushButton#btnActualizarActividades {
            background-color: #187545;
            color: white;
            border: none;
        }

        QPushButton#btnActualizarActividades:hover {
            background-color: #105B34;
        }

        QPushButton#btnCerrarActividades {
            background-color: #7B2835;
            color: white;
            border: none;
        }

        QPushButton#btnCerrarActividades:hover {
            background-color: #621E29;
        }

        QTableWidget {
            background-color: white;
            alternate-background-color: #F2F8F3;
            color: #26382B;
            gridline-color: #E2EAE3;
            border: 1px solid #DDE7DE;
            border-radius: 8px;
            selection-background-color: #D6EBDD;
            selection-color: #174D32;
        }

        QHeaderView::section {
            background-color: #174D32;
            color: white;
            padding: 10px;
            border: none;
            border-right: 1px solid #326448;
            font-weight: bold;
        }

        QLabel#lblTotalActividades {
            font-weight: bold;
            color: #174D32;
        }

        QLabel#lblEstadoActividades {
            color: #63736A;
        }
    )");
}

// ==========================================
// CONFIGURAR TABLA
// ==========================================

void VentanaActividades::configurarTabla()
{
    tablaActividades->setColumnCount(5);

    tablaActividades->setHorizontalHeaderLabels(
        QStringList{
            "ID",
            "Fecha y hora",
            "Usuario",
            "Tipo de actividad",
            "Descripcion"
        }
    );

    // No permitir modificar registros
    tablaActividades->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Seleccionar filas completas
    tablaActividades->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    tablaActividades->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    tablaActividades->setAlternatingRowColors(
        true
    );

    tablaActividades->setWordWrap(true);

    tablaActividades->verticalHeader()->hide();

    tablaActividades->horizontalHeader()
        ->setStretchLastSection(true);

    tablaActividades->setColumnWidth(0, 60);
    tablaActividades->setColumnWidth(1, 170);
    tablaActividades->setColumnWidth(2, 140);
    tablaActividades->setColumnWidth(3, 190);

    tablaActividades->horizontalHeader()
        ->setSectionResizeMode(
            4,
            QHeaderView::Stretch
        );

    tablaActividades->setSortingEnabled(false);
}

// ==========================================
// CARGAR ACTIVIDADES POR FECHA
// ==========================================

void VentanaActividades::cargarActividades()
{
    if (gestorActividades == nullptr)
    {
        actividadesCargadas.clear();

        lblEstado->setText(
            "Error: gestor no disponible"
        );

        aplicarFiltros();

        QMessageBox::warning(
            this,
            "Registro de Actividades",
            "No se encontro el gestor de actividades."
        );

        return;
    }

    // Obtener fecha seleccionada
    string fecha =
        selectorFecha->date()
        .toString("yyyy-MM-dd")
        .toStdString();

    // Consultar registros de esa fecha
    actividadesCargadas =
        gestorActividades->listarPorFecha(fecha);

    // Ordenar del mas reciente al mas antiguo
    sort(
        actividadesCargadas.begin(),
        actividadesCargadas.end(),
        [](const Actividad& a, const Actividad& b)
        {
            if (a.getFechaHora() != b.getFechaHora())
            {
                return a.getFechaHora() >
                    b.getFechaHora();
            }

            return a.getId() > b.getId();
        }
    );

    // Actualizar opciones del filtro
    actualizarTiposActividad();

    // Mostrar resultados
    aplicarFiltros();

    lblEstado->setText(
        "Consulta actualizada: " +
        selectorFecha->date().toString("dd/MM/yyyy")
    );
}

// ==========================================
// ACTUALIZAR TIPOS DE ACTIVIDAD
// ==========================================

void VentanaActividades::actualizarTiposActividad()
{
    // Evitar que el combo dispare filtros
    // mientras se actualizan sus opciones.
    QSignalBlocker bloqueo(cmbTipoActividad);

    QString seleccionAnterior =
        cmbTipoActividad->currentText();

    cmbTipoActividad->clear();

    cmbTipoActividad->addItem(
        "Todas las actividades"
    );

    set<QString> tipos;

    for (const Actividad& actividad : actividadesCargadas)
    {
        tipos.insert(
            QString::fromStdString(
                actividad.getTipo()
            )
        );
    }

    for (const QString& tipo : tipos)
    {
        cmbTipoActividad->addItem(tipo);
    }

    int indice =
        cmbTipoActividad->findText(
            seleccionAnterior
        );

    if (indice >= 0)
    {
        cmbTipoActividad->setCurrentIndex(
            indice
        );
    }
    else
    {
        cmbTipoActividad->setCurrentIndex(0);
    }
}

// ==========================================
// APLICAR FILTROS
// ==========================================

void VentanaActividades::aplicarFiltros()
{
    // Limpiar filas anteriores
    tablaActividades->setRowCount(0);

    QString usuarioBuscado =
        txtBuscarUsuario->text().trimmed();

    QString tipoSeleccionado =
        cmbTipoActividad->currentText();

    int cantidadMostrada = 0;

    for (const Actividad& actividad : actividadesCargadas)
    {
        QString nombreUsuario =
            QString::fromStdString(
                actividad.getNombreUsuario()
            );

        QString tipo =
            QString::fromStdString(
                actividad.getTipo()
            );

        // ==================================
        // FILTRO POR USUARIO
        // ==================================

        if (!usuarioBuscado.isEmpty() &&
            !nombreUsuario.contains(
                usuarioBuscado,
                Qt::CaseInsensitive
            ))
        {
            continue;
        }

        // ==================================
        // FILTRO POR TIPO
        // ==================================

        if (tipoSeleccionado !=
            "Todas las actividades" &&
            tipo != tipoSeleccionado)
        {
            continue;
        }

        // Agregar actividad a la tabla
        agregarFila(actividad);

        cantidadMostrada++;
    }

    // Actualizar contador
    actualizarContador(
        cantidadMostrada
    );
}

// ==========================================
// AGREGAR ACTIVIDAD A LA TABLA
// ==========================================

void VentanaActividades::agregarFila(
    const Actividad& actividad)
{
    int fila =
        tablaActividades->rowCount();

    tablaActividades->insertRow(fila);

    // ======================================
    // ID
    // ======================================

    QTableWidgetItem* itemId =
        new QTableWidgetItem(
            QString::number(
                actividad.getId()
            )
        );

    itemId->setTextAlignment(
        Qt::AlignCenter
    );

    // ======================================
    // FECHA Y HORA
    // ======================================

    QString fechaHora =
        QString::fromStdString(
            actividad.getFechaHora()
        );

    QTableWidgetItem* itemFecha =
        new QTableWidgetItem(fechaHora);

    // ======================================
    // USUARIO
    // ======================================

    QTableWidgetItem* itemUsuario =
        new QTableWidgetItem(
            QString::fromStdString(
                actividad.getNombreUsuario()
            )
        );

    // ======================================
    // TIPO DE ACTIVIDAD
    // ======================================

    QTableWidgetItem* itemTipo =
        new QTableWidgetItem(
            QString::fromStdString(
                actividad.getTipo()
            )
        );

    // ======================================
    // DESCRIPCION
    // ======================================

    QTableWidgetItem* itemDescripcion =
        new QTableWidgetItem(
            QString::fromStdString(
                actividad.getDescripcion()
            )
        );

    // Mostrar descripcion completa
    // al colocar el cursor encima.
    itemDescripcion->setToolTip(
        itemDescripcion->text()
    );

    // ======================================
    // INSERTAR ELEMENTOS
    // ======================================

    tablaActividades->setItem(
        fila, 0, itemId
    );

    tablaActividades->setItem(
        fila, 1, itemFecha
    );

    tablaActividades->setItem(
        fila, 2, itemUsuario
    );

    tablaActividades->setItem(
        fila, 3, itemTipo
    );

    tablaActividades->setItem(
        fila, 4, itemDescripcion
    );

    tablaActividades->resizeRowToContents(
        fila
    );
}

// ==========================================
// ACTUALIZAR CONTADOR
// ==========================================

void VentanaActividades::actualizarContador(
    int cantidadMostrada)
{
    lblTotalActividades->setText(
        QString("Actividades mostradas: %1 de %2")
        .arg(cantidadMostrada)
        .arg(
            static_cast<int>(
                actividadesCargadas.size()
            )
        )
    );
}
