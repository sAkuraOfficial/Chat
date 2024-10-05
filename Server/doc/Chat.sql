CREATE TABLE [dbo].[MSG] (
    [ID]        INT            IDENTITY (1, 1) NOT NULL, -- 主键   自增
    [SENDER]    INT            NOT NULL, -- 发送者
    [CONTENT]   NVARCHAR (MAX) NOT NULL,-- 内容   无限长度
    [SEND_TIME] DATETIME2 (7)  CONSTRAINT [DF_MSG_SENDTIME] DEFAULT (getdate()) NOT NULL,-- 发送时间  默认当前时间
    CONSTRAINT [PK_MSG] PRIMARY KEY CLUSTERED ([ID] ASC),-- 主键
    CONSTRAINT [FK_MSG_USER] FOREIGN KEY ([SENDER]) REFERENCES [dbo].[USERS] ([ID]) ON DELETE CASCADE ON UPDATE CASCADE-- 外键    SEND_ID  关联USER_ID
);

CREATE TABLE [dbo].[USERS] (
    [ID]     INT           IDENTITY (1, 1) NOT NULL,-- 主键   自增
    [NAME]   NVARCHAR (50) NOT NULL,-- 用户名
    [PWD]    NVARCHAR (50) NOT NULL,-- 密码
    [STATUS] NVARCHAR (20) CONSTRAINT [DF_Users_Status] DEFAULT ('offline') NOT NULL,-- 状态   默认离线
    CONSTRAINT [PK_Users_ID] PRIMARY KEY CLUSTERED ([ID] ASC),-- 主键 
    CONSTRAINT [UQ_Users_Name] UNIQUE NONCLUSTERED ([NAME] ASC),-- 唯一约束 用户名唯一
    CONSTRAINT [CK_Users_Status] CHECK (lower([STATUS])='offline' OR lower([STATUS])='online')-- 约束  状态只能为online或offline
);