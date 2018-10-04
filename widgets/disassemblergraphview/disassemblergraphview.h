#ifndef DISASSEMBLERGRAPHVIEW_H
#define DISASSEMBLERGRAPHVIEW_H

#include "../graphview/graphview.h"
#include "../../redasm/disassembler/disassemblerapi.h"

class DisassemblerGraphView : public GraphView
{
    Q_OBJECT

    public:
        explicit DisassemblerGraphView(QWidget *parent = NULL);
        void setDisassembler(REDasm::DisassemblerAPI* disassembler);
        void graph();

    protected:
        virtual QString getNodeContent(const REDasm::Graphing::Node* n);

    private:
        REDasm::DisassemblerAPI* m_disassembler;
};

#endif // DISASSEMBLERGRAPHVIEW_H
