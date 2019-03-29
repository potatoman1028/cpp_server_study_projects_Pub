-- ���̺� ����
CREATE TABLE t_AccountData
	(oidAccount BIGINT IDENTITY(1,1) PRIMARY KEY,
	id NVARCHAR(50) UNIQUE,
	password NVARCHAR(50) NOT NULL,
	createDate DATETIME NULL,
	logoutDate DATETIME NULL)
	GO

-- ���� ������ �ֱ�
CREATE PROC p_AccountData_Insert
@id NVARCHAR(50),
@password NVARCHAR(50)
AS
BEGIN
	DECLARE @count INT
	SELECT @count = COUNT(*) FROM dbo.t_AccountData WHERE id = @id
	IF @count <> 0
		RETURN 0

	INSERT INTO dbo.t_AccountData (id, password, createDate, logoutDate)
	VALUES (@id, @password, GETDATE(), NULL)

	RETURN 1
END
GO
-- ��ȯ �ֱ�
GRANT EXECUTE ON dbo.p_AccountData_Insert To gameServer
GO
-- �⺻ ������ �߰�
EXEC p_AccountData_Insert 'test-01', '1q2w3e'
EXEC p_AccountData_Insert 'test-02', '1q2w3e'
GO

-- ��ȸ ���ν��� ����
CREATE PROC p_AccountData_Select
@id NVARCHAR(50),
@password NVARCHAR(50)
AS
BEGIN
	SELECT id FROM dbo.t_AccountData
	WHERE id = @id and password = @password 

	IF @@ROWCOUNT <> 0
		RETURN 0

	RETURN 1
END
GO
GRANT EXECUTE ON p_AccountData_Select TO gameServer
GO

EXEC p_AccountData_Select 'test-01', '1q2w3e' -- ���� ��
EXEC p_AccountData_Select 'test-02', '1q' -- ���� ��

-------------------------------------------------------------
-- 2�� ������

USE GameDB
GO

CREATE TABLE t_CharacterData
	(oidCharacter BIGINT IDENTITY(1,1) PRIMARY KEY,
	oidAccount BIGINT,
	name NVARCHAR(50) UNIQUE,
	level INT,
	experience BIGINT,
	birthDate DATETIME,
	logoutDate DATETIME,
	worldNumber INT,
	pointX INT,
	pointY INT)
GO

CREATE PROC p_Character_Insert
@oidAccount BIGINT,
@name NVARCHAR(50),
@level INT = 1,
@experience BIGINT = 0
AS
BEGIN
	INSERT dbo.t_CharacterData (oidAccount, name, level, experience, birthDate, logoutDate, worldNumber, pointX, pointY)
	VALUES (@oidAccount, @name, @level, @experience, GETDATE(), GETDATE(), 0, 0, 0)
END
GO
GRANT EXECUTE ON dbo.p_Character_Insert To gameServer
GO

CREATE PROC p_Character_Select
@oidAccountID bigint
AS
BEGIN
	SELECT name, level, experience, birthDate, logoutDate, worldNumber, pointX, pointY 
	FROM dbo.t_CharacterData WITH(NOLOCK)
	WHERE oidAccount = @oidAccountID

	RETURN 1
END
GO
GRANT EXECUTE ON dbo.p_Character_Select To gameServer
GO

CREATE PROC p_Character_Update
@name NVARCHAR(50),
@level INT,
@experience BIGINT,
@worldNumber INT,
@pointX INT,
@pointY INT
AS
BEGIN
	UPDATE dbo.t_CharacterData
	SET level = @level, experience = @experience, logoutDate = GETDATE(),
		worldNumber = @worldNumber, pointX = @pointX, pointY = @pointY
	WHERE name = @name
END
GO
GRANT EXECUTE ON dbo.t_CharacterData To gameServer
GO

-- Ư�� ������ ĳ���� ������ �˻� ���ν���
CREATE PROC p_Account_CharacterData_Select
@oidAccount NVARCHAR(50)
AS
BEGIN
	SELECT name
	FROM dbo.t_AccountData A1
	INNER JOIN dbo.t_CharacterData A2
	ON A1.oidAccount = A2.oidAccount
	WHERE A1.oidAccount = @oidAccount
END
GO

GRANT EXECUTE ON dbo.p_Account_CharacterData_Select To gameServer
GO

-- account ��ȸ ������ ĳ���� �����͸� �����ϵ��� ���ν��� ����
ALTER PROC [dbo].[p_AccountData_Select]
@id NVARCHAR(50),
@password NVARCHAR(50)
AS
BEGIN
	SELECT oidAccount FROM dbo.t_AccountData
	WHERE id = @id and password = @password 

	IF @@ROWCOUNT <> 0
		RETURN 0

	RETURN 1
END
