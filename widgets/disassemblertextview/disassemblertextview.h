#ifndef DISASSEMBLERTEXTVIEW_H
#define DISASSEMBLERTEXTVIEW_H

#include <QAbstractScrollArea>
#include <QTextDocument>
#include <QFontMetrics>
#include <QStack>
#include <QMenu>
#include "listingtextrenderer.h"
#include "disassemblerhighlighter.h"

class DisassemblerTextView : public QAbstractScrollArea
{
    Q_OBJECT

    public:
        enum EmitMode { Normal, VMIL };

    public:
        explicit DisassemblerTextView(QWidget *parent = 0);
        ~DisassemblerTextView();
        bool canGoBack() const;
        bool canGoForward() const;
        address_t currentAddress() const;
        address_t symbolAddress() const;
        void setEmitMode(u32 emitmode);
        void setDisassembler(REDasm::DisassemblerAPI* disassembler);

    public slots:
        void goTo(const REDasm::SymbolPtr& symbol);
        void goTo(address_t address);
        void rename(address_t address);
        void goBack();
        void goForward();

    protected:
        virtual void paintEvent(QPaintEvent* e);
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void keyPressEvent(QKeyEvent *e);

    private:
        void onDisassemblerFinished();
        void onDocumentChanged(const REDasm::ListingDocumentChanged* ldc);

    private:
        int visibleLines() const;
        int lastVisibleLine() const;
        int lineFromPos(const QPoint& p) const;
        bool isLineVisible(int line) const;
        void moveToCurrentLine();
        void createContextMenu();
        void adjustContextMenu();
        void highlightWords();
        void showReferences(address_t address);
        void showCallGraph(address_t address);

    signals:
        void gotoRequested();
        void canGoBackChanged();
        void canGoForwardChanged();
        void hexDumpRequested(address_t address);
        void symbolRenamed(const REDasm::SymbolPtr& symbol);
        void addressChanged(address_t address);

    private:
        bool m_issymboladdressvalid;
        u32 m_emitmode;
        QStack<address_t> m_backstack, m_forwardstack;
        ListingTextRenderer* m_renderer;
        DisassemblerHighlighter* m_highlighter;
        REDasm::DisassemblerAPI* m_disassembler;
        QAction *m_actrename, *m_actcreatestring, *m_actxrefs, *m_actfollow, *m_actcallgraph;
        QAction *m_actgoto, *m_acthexdump, *m_actback, *m_actforward, *m_actcopy, *m_actselectall;
        QMenu* m_contextmenu;
        address_t m_currentaddress, m_symboladdress;
};

#endif // DISASSEMBLERTEXTVIEW_H
