#include "graphview.h"
#include <QWebEngineSettings>
#include <QFontDatabase>
#include <QApplication>
#include "../../themeprovider.h"

#define GRAPH_MARGINS 20

GraphView::GraphView(QWidget *parent): QWebEngineView(parent)
{
    connect(this, &GraphView::loadFinished, this, &GraphView::initializePage);
    this->load(QUrl("qrc:/web/graph.html"));
}

void GraphView::setGraph(const REDasm::Graphing::Graph &graph)
{
    this->page()->runJavaScript("GraphView.initGraph();");

    this->generateNodes(graph);
    this->generateEdges(graph);

    this->page()->runJavaScript(QString("GraphView.renderGraph(%1, %2, %3);")
        .arg(this->width())
        .arg(this->height())
        .arg(GRAPH_MARGINS)
    );
}

QString GraphView::getNodeTitle(const REDasm::Graphing::Node *n) const { Q_UNUSED(n) return QString(); }

QColor GraphView::getEdgeColor(const REDasm::Graphing::Node *from, const REDasm::Graphing::Node *to) const
{
    Q_UNUSED(from)
    Q_UNUSED(to)

    return QColor(Qt::black);
}

void GraphView::zoomOn(int line)
{
    /*
    this->page()->runJavaScript(QString("GraphView.zoomOn(%1);").arg(line));
    */
}

void GraphView::appendCSS(const QString &css)
{
    this->page()->runJavaScript(QString("GraphView.appendCss('%1');").arg(css));
}

void GraphView::initializePage()
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QPalette palette = qApp->palette();

    this->page()->runJavaScript("GraphView.initPage();");

    QString generalcss = "html {"
                             "cursor: default;"
                             "font-family:" + font.family() + ";" +
                             "font-size:" + QString::number(font.pointSize()) + "pt;" +
                             "color:" + palette.color(QPalette::WindowText).name() + ";" +
                             "background-color:" + palette.color(QPalette::Base).name() + ";" +
                         "}"
                         "html, body {"
                            "overflow: hidden;"
                         "}";

    QString blockcss =  ".nodetitle { "
                            "text-align: center;"
                            "margin-bottom: 4px;"
                            "border: 1px solid " + palette.color(QPalette::WindowText).name() + ";"
                            "background-color: " + palette.color(QPalette::Window).name() + ";"
                            "color: " + palette.color(QPalette::WindowText).name() + ";"
                        "}"
                         ".node rect {"
                            "fill: " + palette.color(QPalette::Base).name() + ";" +
                            "stroke: " + palette.color(QPalette::WindowText).name() + ";" +
                            "stroke-width: 3;"
                            "filter: url(#dropshadow);"
                        "}"
                        ".edgePath path {"
                            "stroke-width: 1.5;"
                        "}";

    this->appendCSS(generalcss);
    this->appendCSS(blockcss);
}

QString GraphView::nodeTitle(const REDasm::Graphing::Node *n) const
{
    QString titlecontent = this->getNodeTitle(n);

    if(titlecontent.isEmpty())
        return QString();

    return QString("<div contenteditable=\"false\" class=\"nodetitle\">%1</div>").arg(titlecontent);
}

void GraphView::generateNodes(const REDasm::Graphing::Graph &graph)
{
    for(auto& n : graph)
    {
        QString title = this->nodeTitle(n.get()), content = this->getNodeContent(n.get());

        this->page()->runJavaScript(QString("GraphView.setNode(%1, '%2', '%3');")
            .arg(n->id)
            .arg(title, content)
        );
    }
}

void GraphView::generateEdges(const REDasm::Graphing::Graph &graph)
{
    for(auto& n : graph)
    {
        const REDasm::Graphing::AdjacencyList& edges = graph.edges(n);

        for(auto& e : edges)
        {
            QColor color = this->getEdgeColor(n.get(), e);
            this->page()->runJavaScript(QString("GraphView.setEdge(%1, %2, '%3');")
                .arg(n->id)
                .arg(e->id)
                .arg(color.name())
            );
        }
    }
}
