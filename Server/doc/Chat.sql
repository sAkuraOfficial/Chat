CREATE TABLE [dbo].[MSG] (
    [ID]        INT            IDENTITY (1, 1) NOT NULL, -- ����   ����
    [SENDER]    INT            NOT NULL, -- ������
    [CONTENT]   NVARCHAR (MAX) NOT NULL,-- ����   ���޳���
    [SEND_TIME] DATETIME2 (7)  CONSTRAINT [DF_MSG_SENDTIME] DEFAULT (getdate()) NOT NULL,-- ����ʱ��  Ĭ�ϵ�ǰʱ��
    CONSTRAINT [PK_MSG] PRIMARY KEY CLUSTERED ([ID] ASC),-- ����
    CONSTRAINT [FK_MSG_USER] FOREIGN KEY ([SENDER]) REFERENCES [dbo].[USERS] ([ID]) ON DELETE CASCADE ON UPDATE CASCADE-- ���    SEND_ID  ����USER_ID
);

CREATE TABLE [dbo].[USERS] (
    [ID]     INT           IDENTITY (1, 1) NOT NULL,-- ����   ����
    [NAME]   NVARCHAR (50) NOT NULL,-- �û���
    [PWD]    NVARCHAR (50) NOT NULL,-- ����
    [STATUS] NVARCHAR (20) CONSTRAINT [DF_Users_Status] DEFAULT ('offline') NOT NULL,-- ״̬   Ĭ������
    CONSTRAINT [PK_Users_ID] PRIMARY KEY CLUSTERED ([ID] ASC),-- ���� 
    CONSTRAINT [UQ_Users_Name] UNIQUE NONCLUSTERED ([NAME] ASC),-- ΨһԼ�� �û���Ψһ
    CONSTRAINT [CK_Users_Status] CHECK (lower([STATUS])='offline' OR lower([STATUS])='online')-- Լ��  ״ֻ̬��Ϊonline��offline
);