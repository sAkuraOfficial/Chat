#include "ChatWidget.h"
#include <QStandardItem>

ChatWidget::ChatWidget(friend_info friend_info, QWidget *parent)
    : QWidget(parent), m_friend_info(friend_info)
{
    // 初始化布局和视图
    m_vlayout = new QVBoxLayout(this);
    m_list = new QListView(this);
    m_data_model = new QStandardItemModel(this);

    // 设置委托
    MessageDelegate *delegate = new MessageDelegate(this);
    m_list->setItemDelegate(delegate);
    m_list->setModel(m_data_model);

    // 配置视图
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setUniformItemSizes(true);

    // 将列表视图添加到布局
    m_vlayout->addWidget(m_list);
    this->setLayout(m_vlayout);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::AddMessage(message_info message)
{
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(message), Qt::UserRole);
    item->setEditable(false);
    m_data_model->appendRow(item);
}
