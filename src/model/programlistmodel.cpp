#include "programlistmodel.h"

ProgramListModel::ProgramListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *reader) :
    QAbstractListModel(parent)
{
    m_dbmanager = dbmanager;
    m_reader = reader;

    //m_reader->updateTVGuide();

}

ProgramListModel::~ProgramListModel() {

    foreach(ProgramData* program, m_programs) {
        delete program;
        program = 0;
    }

    qDebug() << "destructing ProgramListModel";
}

QHash<int,QByteArray> ProgramListModel::roleNames() {

    QHash<int, QByteArray> roles;
    roles[ChannelNameRole] = "channelName";
    roles[ProgramNameRole] = "programName";
    roles[TimeRole] = "time";
    roles[WeekdayRole] = "weekday";
    roles[OverviewRole] = "overview";
    return roles;
}

void ProgramListModel::addProgram(ProgramData *program) {

    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_programs.append(program);
    endInsertRows();
}

QVariant ProgramListModel::data(const QModelIndex &index, int role) const {

    if(!index.isValid() || index.row() >= m_programs.count()) {
        return QVariant();
    }

    ProgramData* program = m_programs.at(index.row());

    switch (role) {
    case ChannelNameRole:
        return program->getChannelName();
    case ProgramNameRole:
        return program->getProgramName();
    case TimeRole:
        return program->getTime();
    case WeekdayRole:
        return program->getWeekday();
    case OverviewRole:
        return program->getOverview();
    default:
        qDebug() << "Unhandled role" << role;
        return QVariant();
    }
}

int ProgramListModel::rowCount(const QModelIndex &index) const {

    Q_UNUSED(index)
    return m_programs.size();
}

QVariantMap ProgramListModel::get(const QModelIndex &index) const {
    Q_UNUSED(index)
    return QVariantMap();
}

void ProgramListModel::setProgramList(QList<ProgramData> programs) {

    Q_UNUSED(programs)

//    if (m_programs.count() > 0) {
//        beginRemoveRows(QModelIndex(), 0, m_programs.count() - 1);
//        m_programs.clear();
//        endRemoveRows();
//    }

//    if (programs.count() > 0) {
//        beginInsertRows(QModelIndex(), 0, m_programs.count() - 1);
//        m_programs << programs;
//        endInsertRows();
//    }

//    emit countChanged();
}

void ProgramListModel::clear() {

    beginResetModel();
    m_programs.clear();
    endResetModel();
}

void ProgramListModel::populateChannel() {

    QString channelname = "MTV3";
    QString programname = "salkkarit";
    QString time = "19:30";
    QString weekday = "keskiviikko";
    QString overview = "ohmygod";

    ProgramData* program1 = new ProgramData(this,programname,weekday,time,channelname,overview);
    ProgramData* program2 = new ProgramData(this,programname,weekday,time,channelname,overview);
    ProgramData* program3 = new ProgramData(this,programname,weekday,time,channelname,overview);
    addProgram(program1);
    addProgram(program2);
    addProgram(program3);
    emit countChanged();

}
