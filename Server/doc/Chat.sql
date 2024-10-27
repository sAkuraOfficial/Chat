-- 创建用户表
CREATE TABLE [dbo].[USERS] (
    [ID]     INT           IDENTITY (1, 1) NOT NULL,-- 主键   自增
    [NAME]   NVARCHAR (50) NOT NULL,-- 用户名
    [PWD]    NVARCHAR (50) NOT NULL,-- 密码
    [STATUS] NVARCHAR (20) CONSTRAINT [DF_Users_Status] DEFAULT ('offline') NOT NULL,-- 状态   默认离线
    CONSTRAINT [PK_Users_ID] PRIMARY KEY CLUSTERED ([ID] ASC),-- 主键 
    CONSTRAINT [UQ_Users_Name] UNIQUE NONCLUSTERED ([NAME] ASC),-- 唯一约束 用户名唯一
    CONSTRAINT [CK_Users_Status] CHECK (lower([STATUS])='offline' OR lower([STATUS])='online')-- 约束  状态只能为online或offline
);

-- 好友之间的关系
CREATE TABLE [dbo].[FRIEND_STATUS] (
    [STATUS_CODE] NVARCHAR(20) PRIMARY KEY,
    [DESCRIPTION] NVARCHAR(50) NOT NULL
);

-- 插入初始状态
INSERT INTO [dbo].[FRIEND_STATUS] (STATUS_CODE, DESCRIPTION) VALUES 
    ('pending', '待确认'),
    ('accepted', '已接受'),
    ('blocked', '屏蔽');


-- 好友表，用户1与用户2两个外键作为联合主键
CREATE TABLE [dbo].[FRIENDS] (
    [USER1_ID] INT NOT NULL,
    [USER2_ID] INT NOT NULL,
    [STATUS] NVARCHAR(20) NOT NULL,
    CONSTRAINT [PK_Friends] PRIMARY KEY ([USER1_ID], [USER2_ID]),
    CONSTRAINT [FK_Friends_User1] FOREIGN KEY ([USER1_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_User2] FOREIGN KEY ([USER2_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_Status] FOREIGN KEY ([STATUS]) REFERENCES [dbo].[FRIEND_STATUS]([STATUS_CODE]),
    CONSTRAINT [CK_Friends_Ordered] CHECK ([USER1_ID] < [USER2_ID]) -- 确保 USER1_ID 小于 USER2_ID
);
