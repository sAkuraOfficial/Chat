#include "ChatMessageList.h"
#include <QEasingCurve>
#include <QScroller>
#include <QStandardItem>
#include<qscrollbar.h>
ChatMessageList::ChatMessageList(friend_info friend_info, QWidget *parent)
    : QWidget(parent), m_friend_info(friend_info)
{
    // 设置meta：friend_id
    this->setProperty("friend_id", m_friend_info.user_id);

    // 初始化布局和视图
    m_vlayout = new QVBoxLayout(this);
    m_list = new QListView(this);
    m_list->setObjectName("listView_message");
    m_data_model = new QStandardItemModel(this);

    // 设置委托
    MessageDelegate *delegate = new MessageDelegate(this);
    m_list->setItemDelegate(delegate);
    m_list->setModel(m_data_model);

    // 配置视图
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setUniformItemSizes(false);
    m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);   // 垂直平滑滚动
    m_list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel); // 水平平滑滚动

    // 调整滚动条单步步长
    m_list->verticalScrollBar()->setSingleStep(10); // 每次滚动 10 像素
    m_list->horizontalScrollBar()->setSingleStep(10);

    // 将列表视图添加到布局
    m_vlayout->addWidget(m_list);
    this->setLayout(m_vlayout);
    this->setStyleSheet(R"(
    #listView_message{
        background-color:rgb(245, 245, 245);
        border:0px;
    }
    QScrollBar:vertical {
        background: #f0f0f0; /* 背景颜色 */
        width: 10px; /* 滚动条宽度 */
        margin: 22px 0 22px 0; /* 上下边距 */
    }

    QScrollBar::handle:vertical {
        background: #c0c0c0; /* 滑块颜色 */
        border-radius: 5px; /* 圆角 */
    }

    QScrollBar::handle:vertical:hover {
        background: #a0a0a0; /* 悬停时滑块颜色 */
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        background: none; /* 隐藏上下按钮 */
    }

    QScrollBar:horizontal {
        background: #f0f0f0; /* 背景颜色 */
        height: 10px; /* 滚动条高度 */
        margin: 0 22px 0 22px; /* 左右边距 */
    }

    QScrollBar::handle:horizontal {
        background: #c0c0c0; /* 滑块颜色 */
        border-radius: 5px; /* 圆角 */
    }

    QScrollBar::handle:horizontal:hover {
        background: #a0a0a0; /* 悬停时滑块颜色 */
    }

    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
        background: none; /* 隐藏左右按钮 */
    }
    )");
}

ChatMessageList::~ChatMessageList()
{
}

void ChatMessageList::AddMessage(message_info message)
{
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(message), Qt::UserRole);
    item->setEditable(false);
    m_data_model->appendRow(item);
}

int ChatMessageList::getFriendId()
{
    return m_friend_info.user_id;
}
void ChatMessageList::wheelEvent(QWheelEvent *event)
{
    if (m_list)
    {
        // 获取滚动增量（通常是 120 的倍数，表示一个滚轮刻度）
        int delta = event->angleDelta().y();

        // 将增量转换为像素滚动
        int step = 15; // 每次滚轮滚动时的步长（像素单位，可以根据需要调整）
        m_list->verticalScrollBar()->setValue(m_list->verticalScrollBar()->value() - delta / 120 * step);

        event->accept();
    }
    else
    {
        QWidget::wheelEvent(event);
    }
}