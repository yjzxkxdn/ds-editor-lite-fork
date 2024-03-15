#include "PhonicModel.h"

namespace FillLyric {
    // Gui functions
    void PhonicModel::repaintItem(const QModelIndex &index, const QString &text) const {
        Q_EMIT dynamic_cast<PhonicDelegate *>(this->m_tableView->itemDelegate())
            ->lyricEdited(index, text);
    }

    int PhonicModel::shrinkModel() {
        // 从右到左遍历所有行，找到最长的一行，赋值给modelMaxCol
        int maxCol = 0;
        for (int i = 0; i < this->rowCount(); i++) {
            maxCol = std::max(maxCol, currentLyricLength(i));
        }
        this->setColumnCount(maxCol);
        return maxCol;
    }

    void PhonicModel::expandModel(const int col) {
        const int maxCol = this->columnCount();
        this->setColumnCount(maxCol + col);
    }

    // Basic functions
    int PhonicModel::currentLyricLength(const int row) const {
        for (int i = this->columnCount() - 1; i >= 0; i--) {
            if (!cellLyric(row, i).isEmpty()) {
                return i + 1;
            }
        }
        return 0;
    }

    void PhonicModel::refreshTable() {
        this->shrinkPhonicList();
        const int m_scrollBarValue = m_tableView->verticalScrollBar()->value();
        const int col = this->columnCount();
        int row = static_cast<int>(this->m_phonics.size() / col);
        if (this->m_phonics.size() % col != 0) {
            row++;
        }

        for (int i = static_cast<int>(this->m_phonics.size()); i < row * col; i++) {
            this->m_phonics.append(Phonic());
        }

        this->clear();
        this->setRowCount(row);
        this->setColumnCount(col);

        for (int i = 0; i < this->m_phonics.size(); i++) {
            this->putData(i / col, i % col, this->m_phonics[i]);
        }
        m_tableView->verticalScrollBar()->setValue(m_scrollBarValue);
    }

    void PhonicModel::shrinkPhonicList() {
        for (int i = static_cast<int>(this->m_phonics.size()) - 1; i >= 0; i--) {
            if (!this->m_phonics[i].lyric.isEmpty()) {
                this->m_phonics.resize(i + 1);
                break;
            }
        }
    }

    // RoleData functions
    QList<int> PhonicModel::allRoles() {
        QList<int> roles;
        roles.append(Qt::DisplayRole);
        for (int i = PhonicRole::Syllable; i <= PhonicRole::FermataAddition; i++) {
            roles.append(i);
        }
        return roles;
    }

    QString PhonicModel::cellLyric(const int row, int col) const {
        if (abs(col) >= this->columnCount()) {
            return {};
        }
        if (col < 0) {
            col = this->columnCount() + col;
        }
        return this->data(this->index(row, col), Qt::DisplayRole).toString();
    }

    bool PhonicModel::setLyric(const int row, const int col, const QString &lyric) {
        this->setData(this->index(row, col), lyric, Qt::DisplayRole);
        return true;
    }

    QString PhonicModel::cellSyllable(const int row, const int col) const {
        return this->data(this->index(row, col), PhonicRole::Syllable).toString();
    }

    bool PhonicModel::setSyllable(const int row, const int col, const QString &syllable) {
        this->setData(this->index(row, col), syllable, PhonicRole::Syllable);
        return true;
    }

    QString PhonicModel::cellSyllableRevised(const int row, const int col) const {
        return this->data(this->index(row, col), PhonicRole::SyllableRevised).toString();
    }

    bool PhonicModel::setSyllableRevised(const int row, const int col,
                                         const QString &syllableRevised) {
        this->setData(this->index(row, col), syllableRevised, PhonicRole::SyllableRevised);
        return true;
    }

    QStringList PhonicModel::cellCandidates(const int row, const int col) const {
        return this->data(this->index(row, col), PhonicRole::Candidate).toStringList();
    }

    bool PhonicModel::setCandidates(const int &row, const int &col, const QStringList &candidate) {
        this->setData(this->index(row, col), candidate, PhonicRole::Candidate);
        return true;
    }

    QString PhonicModel::cellLyricType(const int row, const int col) const {
        return this->data(this->index(row, col), PhonicRole::Language).toString();
    }

    bool PhonicModel::setLyricType(const int row, const int col, const QString &type) {
        this->setData(this->index(row, col), type, PhonicRole::Language);
        return true;
    }

    QStringList PhonicModel::cellFermata(const int row, const int col) const {
        return this->data(this->index(row, col), PhonicRole::FermataAddition).toStringList();
    }

    bool PhonicModel::setFermata(const int row, const int col, const QStringList &fermata) {
        this->setData(this->index(row, col), fermata, PhonicRole::FermataAddition);
        return true;
    }

    QString PhonicModel::cellToolTip(const int &row, const int &col) const {
        return this->data(this->index(row, col), Qt::ToolTipRole).toString();
    }

    // Cell operations
    void PhonicModel::putData(const int row, const int col, const Phonic &phonic) {
        // 将phonic的内容放到row行col列
        setLyric(row, col, phonic.lyric);
        if (phonic.lyric.isEmpty()) {
            return;
        }
        setSyllable(row, col, phonic.syllable);
        setCandidates(row, col, phonic.candidates);
        setSyllableRevised(row, col, phonic.syllableRevised);
        setLyricType(row, col, phonic.language);
        setFermata(row, col, phonic.fermata);
    }

    Phonic PhonicModel::takeData(const int row, const int col) const {
        Phonic phonic;
        // 根据span的包含的角色，将row行col列的数据取出
        phonic.lyric = cellLyric(row, col);
        if (phonic.lyric.isEmpty()) {
            return phonic;
        }
        phonic.syllable = cellSyllable(row, col);
        phonic.candidates = cellCandidates(row, col);
        phonic.syllableRevised = cellSyllableRevised(row, col);
        phonic.language = cellLyricType(row, col);
        phonic.fermata = cellFermata(row, col);
        return phonic;
    }

    void PhonicModel::clearData(const int row, const int col) {
        auto roles = allRoles();
        // 根据span的包含的角色，将row行col列的数据清空
        for (const int role : roles) {
            this->setData(this->index(row, col), QVariant(), role);
        }
    }

    void PhonicModel::moveData(const int row, const int col, const int tarRow, const int tarCol) {
        auto roles = allRoles();
        if (row == tarRow && col == tarCol) {
            return;
        }
        // 源数据超出范围，目标数据在范围内，清空目标数据
        if (col >= this->columnCount() && this->index(tarRow, tarCol).isValid()) {
            clearData(tarRow, tarCol);
        }
        // 源数据在范围内，目标数据超出范围，扩展模型
        if (tarRow >= this->rowCount()) {
            for (int i = this->rowCount(); i <= tarRow; i++) {
                this->insertRow(i);
            }
        }
        if (tarCol >= this->columnCount()) {
            expandModel(tarCol - this->columnCount() + 1);
        }
        // 根据span的包含的角色，将row行col列的数据移动到tarRow行tarCol列
        for (const int role : roles) {
            this->setData(this->index(tarRow, tarCol), this->data(this->index(row, col), role),
                          role);
            this->setData(this->index(row, col), QVariant(), role);
        }
    }

    void PhonicModel::insertWarpCell(const int &index, const Phonic &phonic) {
        if (index > m_phonics.size()) {
            for (int i = static_cast<int>(m_phonics.size()); i < index; i++) {
                m_phonics.append(Phonic());
            }
        }
        m_phonics.insert(index, phonic);
    }

    void PhonicModel::editWarpCell(const int &index, const Phonic &phonic) {
        if (index >= m_phonics.size()) {
            qWarning() << "PhonicModel::editWarpCell: index out of range";
            return;
        }
        m_phonics[index] = phonic;
    }

    void PhonicModel::deleteWarpCell(const int &index) {
        if (index >= m_phonics.size()) {
            qWarning() << "PhonicModel::deleteWarpCell: index out of range";
            return;
        }
        m_phonics.removeAt(index);
    }

    void PhonicModel::putCell(const QModelIndex &index, const Phonic &phonic) {
        // 获取当前单元格坐标
        const int row = index.row();
        const int col = index.column();
        // 将phonic的内容放到当前单元格
        putData(row, col, phonic);
    }

    Phonic PhonicModel::takeCell(const QModelIndex &index) const {
        // 获取当前单元格坐标
        const int row = index.row();
        const int col = index.column();
        // 获取当前单元格的内容
        return takeData(row, col);
    }

    void PhonicModel::clearCell(const QModelIndex &index) {
        // 清空当前单元格
        clearData(index.row(), index.column());
    }

    void PhonicModel::moveCell(const QModelIndex &source, const QModelIndex &target) {
        // 获取当前单元格坐标
        const int row = source.row();
        const int col = source.column();
        // 将source的内容移动到target
        moveData(row, col, target.row(), target.column());
    }

    QString PhonicModel::cellToolTip(const QModelIndex &index) const {
        return this->data(index, Qt::ToolTipRole).toString();
    }

    void PhonicModel::cellMoveLeft(const QModelIndex &index) {
        // 获取当前单元格坐标
        const int row = index.row();
        const int col = index.column();
        // 将当前的单元格的内容移动到左边的单元格，右边单元格的内容依次向左移动
        for (int i = col; 0 < i && i < this->columnCount(); i++) {
            moveData(row, i, row, i - 1);
        }
    }

    void PhonicModel::cellMoveRight(const QModelIndex &index) {
        //  获取当前单元格坐标
        const int row = index.row();
        const int col = index.column();
        // 将对应的单元格的内容移动到右边的单元格，右边单元格的内容依次向右移动，超出范围的部分向右新建单元格
        QString lastLyric = cellLyric(row, -1);
        if (!lastLyric.isEmpty()) {
            expandModel(1);
        }

        // 向右移动
        for (int i = this->columnCount() - 1; i > col; i--) {
            moveData(row, i - 1, row, i);
        }
    }

    // Fermata operations
    void PhonicModel::collapseFermata() {
        // 遍历模型每行
        for (int row = 0; row < this->rowCount(); row++) {
            int pos = 1;
            while (pos < currentLyricLength(row)) {
                // 获取当前单元格的内容
                auto currentType = cellLyricType(row, pos);
                if (currentType == "Slur") {
                    const int start = pos;
                    while (pos < this->columnCount() && cellLyricType(row, pos) == "Slur") {
                        pos++;
                    }

                    // 把pos-1的单元格的FermataRole设为折叠的FermataList
                    QStringList fermataList;
                    for (int j = start; j < pos; j++) {
                        fermataList.append(cellLyric(row, j));
                    }
                    setFermata(row, start - 1, fermataList);

                    // 右侧数据左移、覆盖延音符号
                    for (int k = 0; k < fermataList.size(); k++) {
                        cellMoveLeft(this->index(row, pos - k));
                    }
                    pos = 1;
                } else {
                    pos++;
                }
            }
        }
    }

    void PhonicModel::expandFermata() {
        // 遍历模型每行
        for (int row = 0; row < this->rowCount(); row++) {
            int pos = 0;
            // 遍历每行的每个单元格
            while (pos < this->columnCount()) {
                // 获取当前单元格的FermataRole的内容
                auto fermataList = cellFermata(row, pos);

                if (!fermataList.isEmpty()) {
                    // 在右侧插入空白单元格
                    if (pos + fermataList.size() > this->columnCount() - 1) {
                        expandModel(pos + static_cast<int>(fermataList.size()) -
                                    (this->columnCount() - 1));
                    } else {
                        for (int j = 0; j < fermataList.size(); j++) {
                            cellMoveRight(this->index(row, pos + 1));
                        }
                    }
                    // 将pos右侧的fermataList.size()个单元格的内容设置为fermataList[j]
                    for (int j = 0; j < fermataList.size(); j++) {
                        setLyric(row, pos + j + 1, fermataList[j]);
                        setSyllable(row, pos + j + 1, fermataList[j]);
                        setCandidates(row, pos + j + 1, QStringList() << fermataList[j]);
                        setLyricType(row, pos + j + 1, "Slur");
                    }
                    // 清空pos的FermataRole
                    setFermata(row, pos, QStringList());
                    pos = 0;
                }
                pos++;
            }
        }
    }

} // FillLyric