#include "AddPrevLineCmd.h"

namespace FillLyric {
    AddPrevLineCmd::AddPrevLineCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent)
        : QUndoCommand(parent), m_view(view), m_newList(cellList) {
        m_index = static_cast<int>(m_view->cellLists().indexOf(cellList));
        m_newList = m_view->createNewList();

        for (const auto &cell : m_newList->m_cells) {
            m_newList->sence()->removeItem(cell);
        }
    }

    void AddPrevLineCmd::undo() {
        m_view->removeList(m_index);
    }

    void AddPrevLineCmd::redo() {
        m_view->insertList(m_index, m_newList);
    }
} // FillLyric