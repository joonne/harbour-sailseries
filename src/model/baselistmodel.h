#ifndef BASELISTMODEL_H
#define BASELISTMODEL_H

#include <QQmlListProperty>

template <class T>
class BaseListModel
{
public:
    explicit BaseListModel()
    {
    }

    ~BaseListModel()
    {
        for (auto item : this->m_listModel)
        {
            delete item;
            item = 0;
        }
    }

    QQmlListProperty<T> getList()
    {
        return QQmlListProperty<T>(this, &m_listModel, &BaseListModel::count, &BaseListModel::at);
    }

protected:
    QList<T*> m_listModel;

private:
    static void append(QQmlListProperty<T>* prop, T* val)
    {
        BaseListModel<T>* listModel = qobject_cast<BaseListModel<T>*>(prop->object);
        listModel->m_listModel.append(val);
    }

    static T* at(QQmlListProperty<T>* prop, int index)
    {
        return (qobject_cast<BaseListModel*>(prop->object))->m_listModel.at(index);
    }

    static int count(QQmlListProperty<T>* prop)
    {
        return qobject_cast<BaseListModel*>(prop->object)->m_listModel.size();
    }

    static void clear(QQmlListProperty<T>* prop)
    {
        qobject_cast<BaseListModel*>(prop->object)->m_listModel.clear();
    }
};

#endif // BASELISTMODEL_H
