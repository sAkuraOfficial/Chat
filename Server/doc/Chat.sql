-- �����û���
CREATE TABLE [dbo].[USERS] (
    [ID]     INT           IDENTITY (1, 1) NOT NULL,-- ����   ����
    [NAME]   NVARCHAR (50) NOT NULL,-- �û���
    [PWD]    NVARCHAR (50) NOT NULL,-- ����
    [STATUS] NVARCHAR (20) CONSTRAINT [DF_Users_Status] DEFAULT ('offline') NOT NULL,-- ״̬   Ĭ������
    CONSTRAINT [PK_Users_ID] PRIMARY KEY CLUSTERED ([ID] ASC),-- ���� 
    CONSTRAINT [UQ_Users_Name] UNIQUE NONCLUSTERED ([NAME] ASC),-- ΨһԼ�� �û���Ψһ
    CONSTRAINT [CK_Users_Status] CHECK (lower([STATUS])='offline' OR lower([STATUS])='online')-- Լ��  ״ֻ̬��Ϊonline��offline
);

-- ����֮��Ĺ�ϵ
CREATE TABLE [dbo].[FRIEND_STATUS] (
    [STATUS_CODE] NVARCHAR(20) PRIMARY KEY,
    [DESCRIPTION] NVARCHAR(50) NOT NULL
);

-- �����ʼ״̬
INSERT INTO [dbo].[FRIEND_STATUS] (STATUS_CODE, DESCRIPTION) VALUES 
    ('pending', '��ȷ��'),
    ('accepted', '�ѽ���'),
    ('blocked', '����');


-- ���ѱ��û�1���û�2���������Ϊ��������
CREATE TABLE [dbo].[FRIENDS] (
    [USER1_ID] INT NOT NULL,
    [USER2_ID] INT NOT NULL,
    [STATUS] NVARCHAR(20) NOT NULL,
    CONSTRAINT [PK_Friends] PRIMARY KEY ([USER1_ID], [USER2_ID]),
    CONSTRAINT [FK_Friends_User1] FOREIGN KEY ([USER1_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_User2] FOREIGN KEY ([USER2_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_Status] FOREIGN KEY ([STATUS]) REFERENCES [dbo].[FRIEND_STATUS]([STATUS_CODE]),
    CONSTRAINT [CK_Friends_Ordered] CHECK ([USER1_ID] < [USER2_ID]) -- ȷ�� USER1_ID С�� USER2_ID
);
