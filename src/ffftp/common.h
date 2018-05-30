/*=============================================================================
*							�e�e�e�s�o���ʒ�`�t�@�C��
*
===============================================================================
/ Copyright (C) 1997-2007 Sota. All rights reserved.
/
/ Redistribution and use in source and binary forms, with or without 
/ modification, are permitted provided that the following conditions 
/ are met:
/
/  1. Redistributions of source code must retain the above copyright 
/     notice, this list of conditions and the following disclaimer.
/  2. Redistributions in binary form must reproduce the above copyright 
/     notice, this list of conditions and the following disclaimer in the 
/     documentation and/or other materials provided with the distribution.
/
/ THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
/ IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
/ OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
/ IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
/ INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
/ USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
/ ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
/ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
/ THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/============================================================================*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include "config.h"
#ifndef ENGLISH
#include "mesg-jpn.h"
#else
#include "mesg-eng.h"
#endif


#define NUL				'\0'

#define LOW8(x)			((x) & 0xFF)
#define HIGH8(x)		(((x) >> 8) & 0xFF)
#define LOW32(x)		((x) & 0xFFFFFFFF)
#define HIGH32(x)		(((x) >> 32) & 0xFFFFFFFF)
#define N2INT(h,l)		((int)(((uchar)(h) << 4) + (uchar)(l)))

#define IsDigit(n)		(isascii(n) && isdigit(n))
#define IsAlpha(n)		(isascii(n) && isalpha(n))

#define uchar			unsigned char
#define ushort			unsigned short
#define ulong			unsigned long

#define FAIL			0
#define SUCCESS			1

#define NO				0
#define YES				1
#define NO_ALL			2
#define YES_ALL			3
#define YES_LIST		4

/*===== �o�[�W���� ======*/

#define VER_STR					"1.97b"
#define VER_NUM					1921		/* �ݒ�o�[�W���� */
#define PROGRAM_VERSION_NUM		1972		/* �o�[�W���� */


#define MYWEB_URL	"http://www2.biglobe.ne.jp/~sota/ffftp-qa.html"



/*===== �ʐM�֌W ======*/

#define TCP_PORT		6

/*===== �E�C���h�E�T�C�Y ======*/

#define TOOLWIN_HEIGHT	28		/* �c�[���o�[�̍��� */

/*===== ����ȃL���b�V���f�[�^�ԍ� =====*/
/* �i�t�@�C���ꗗ�擾�Ŏg�p���郍�[�J���t�@�C���� _ffftp.??? �̔ԍ������j */

#define CACHE_FILE_TMP1	999		/* �z�X�g�̃t�@�C���c���[�擾�p */
#define CACHE_FILE_TMP2	998		/* �A�b�v���[�h���̃z�X�g�̃t�@�C���ꗗ�擾�p */

/*===== ���[�U��`�R�}���h =====*/

#define WM_CHANGE_COND	(WM_USER+1)	/* �t�@�C���ꗗ��ύX����R�}���h */
#define WM_SET_PACKET	(WM_USER+2)	/* ���ݎg�p���Ă���]���p�P�b�g�̃A�h���X��ʒm */
#define WM_SELECT_HOST	(WM_USER+3)	/* �z�X�g���_�u���N���b�N�őI������ */
#define WM_DIAL_MSG		(WM_USER+4)	/* �_�C�A�����̃X�e�[�^�X�ʒm */

#define WM_ASYNC_SOCKET	(WM_USER+5)
#define WM_ASYNC_DBASE	(WM_USER+6)

#define WM_REFRESH_LOCAL_FLG	(WM_USER+7)
#define WM_REFRESH_REMOTE_FLG	(WM_USER+8)

/*===== �z�X�g�ԍ� =====*/
/* �z�X�g�ԍ��� 0�` �̒l����� */

#define HOSTNUM_NOENTRY	(-1)	/* �z�X�g�ꗗ�ɖ����Ƃ��̃z�X�g�ԍ� */

/*===== �o�b�t�@�T�C�Y =====*/

#define BUFSIZE			4096	/* �t�@�C���]���o�b�t�@�̃T�C�Y(4k�ȏ�) */

#define HOST_NAME_LEN	40		/* �ꗗ�ɕ\������z�X�g�̖��O */
#define HOST_ADRS_LEN	80		/* �z�X�g�� */
#define USER_NAME_LEN	80		/* ���[�U�� */
#define PASSWORD_LEN	80		/* �p�X���[�h */
#define ACCOUNT_LEN		80		/* �A�J�E���g */
#define HOST_TYPE_LEN	1		/* �z�X�g�̎�� */
#define INIT_DIR_LEN	(FMAX_PATH-40)	/* �����f�B���N�g�� */
#define USER_MAIL_LEN	80		/* ���[�U�̃��[���A�h���X */
								/*   PASSWORD_LEN �Ɠ����ɂ��邱�� */
#define ASCII_EXT_LEN	400		/* �A�X�L�[���[�h�]���̃t�@�C������ */
#define FILTER_EXT_LEN	400		/* �t�B���^�̃t�@�C������ */
#define BOOKMARK_SIZE	2048	/* �u�b�N�}�[�N */
#define CHMOD_CMD_LEN	40		/* �����ύX�R�}���h */
#define MIRROR_LEN		400		/* �~���[�����O�̐ݒ�p */
#define NLST_NAME_LEN	40		/* NLST�ɕt����t�@�C�����^�I�v�V���� */
#define DEFATTRLIST_LEN	800		/* �������X�g�̒��� */
#define INITCMD_LEN		256		/* �������R�}���h */
#define OWNER_NAME_LEN	40		/* �I�[�i�� */
#define RAS_NAME_LEN	256		/* RAS�̃G���g�����̒��� */

#define FMAX_PATH		1024

#define ONELINE_BUF_SIZE	(10*1024)

/*===== �����l =====*/

#define SAMBA_HEX_TAG	':'				/* Samba-HEX �̋�؂蕶�� */
#define CHMOD_CMD_NOR	"SITE CHMOD"	/* �����ύX�R�}���h */
#define PORT_NOR		21				/* �|�[�g�ԍ� */
#define LS_FNAME		"-alL"			/* NLST�ɕt������� */

/*===== �������O�̃t�@�C�������������̏��� =====*/

#define EXIST_OVW		0		/* �㏑�� */
#define EXIST_NEW		1		/* �V������Ώ㏑�� */
#define EXIST_RESUME	2		/* ���W���[�� */
#define EXIST_IGNORE	3		/* ���� */
#define EXIST_UNIQUE	4		/* �z�X�g�����O��t���� */
#define EXIST_ABORT		5		/* �S�Ē��~ */

/*===== �t�@�C�����̔�r���[�h =====*/

#define COMP_IGNORE		0		/* �啶��/�������͋�ʂ��Ȃ� */
#define COMP_STRICT		1		/* �啶��/����������ʂ��� */
#define COMP_LOWERMATCH	2		/* �啶��/����������ʂ��Ȃ��i�Б��͑S�ď������j */

/*===== FTP�̉����R�[�h�̓��P�� =====*/

#define FTP_PRELIM		1		/* */
#define FTP_COMPLETE	2		/* */
#define FTP_CONTINUE	3		/* */
#define FTP_RETRY		4		/* */
#define FTP_ERROR		5		/* */

/*===== �t�@�C�����X�g�̃m�[�h���� =====*/

#define NODE_DIR		0		/* �f�B���N�g�� */
#define NODE_FILE		1		/* �t�@�C�� */
#define NODE_DRIVE		2		/* �h���C�u */
#define NODE_NONE		(-1)	/* �Ȃ� */

/*===== �㏑���m�F���@�̐ݒ�l =====*/

#define TRANS_OVW		0		/* �㏑�� */
#define TRANS_DLG		1		/* �_�C�A���O���o�� */

/*===== �z�X�g���t�@�C���ړ��m�F���@�̐ݒ�l =====*/

#define MOVE_NODLG		0		/* �_�C�A���O���o���Ȃ� */
#define MOVE_DLG		1		/* �_�C�A���O���o�� */
#define MOVE_DISABLE	2		/* �@�\�g�p�֎~ */

/*===== �]�����@ =====*/

#define TYPE_I			'I'		/* �o�C�i�����[�h */
#define TYPE_A			'A'		/* �A�X�L�[���[�h */
#define TYPE_X			'X'		/* �������� */

#define TYPE_DEFAULT	NUL		/* �ݒ肵�Ȃ� DirectConnectProc()�̈����Ƃ��ėL�� */

/*===== �\�[�g���@ =====*/
/* �ԍ���ListView�̃J�����ԍ��ƍ��킹�Ă��� */

#define SORT_NAME		0		/* ���O�� */
#define SORT_DATE		1		/* ���t�� */
#define SORT_SIZE		2		/* �T�C�Y�� */
#define SORT_EXT		3		/* �g���q�� */

#define SORT_ASCENT		0x00	/* ���� */
#define SORT_DESCENT	0x80	/* �~�� */

#define SORT_MASK_ORD	0x7F	/* �����������o���}�X�N */
#define SORT_GET_ORD	0x80	/* �����^�~�������o���}�X�N */

#define SORT_NOTSAVED	((ulong)0xFFFFFFFF)	/* �z�X�g���̃Z�[�u���@��ۑ����Ă��Ȃ����̒l */

/*===== �\�[�g����ꏊ =====*/

#define ITEM_LFILE		0		/* ���[�J���̖��O */
#define ITEM_LDIR		1		/* ���[�J���̃f�B���N�g�� */
#define ITEM_RFILE		2		/* �z�X�g�̖��O */
#define ITEM_RDIR		3		/* �z�X�g�̃f�B���N�g�� */

/*===== �E�C���h�E�ԍ� =====*/

#define WIN_LOCAL		0		/* ���[�J�� */
#define WIN_REMOTE		1		/* �z�X�g */
#define WIN_BOTH		2		/* ���� */

/*===== �t�@�C���I����@ =====*/

#define SELECT_ALL		0		/* �S�I�� */
#define SELECT_REGEXP	1		/* ����������͂��đI�� */

/*===== �������@ =====*/

#define FIND_FIRST		0		/* �ŏ��̌��� */
#define FIND_NEXT		1		/* �������� */

/*===== �t�@�C�����̑啶���^�������ϊ� =====*/

#define FNAME_NOCNV		0		/* �ϊ����Ȃ� */
#define FNAME_LOWER		1		/* �������ɕϊ� */
#define FNAME_UPPER		2		/* �啶���ɕϊ� */

/*===== �t�@�C���ꗗ�̌`�� =====*/

#define LIST_UNKNOWN	(-1)	/* �s�� */

#define LIST_UNIX_10	0		/* UNIX 10 */
#define LIST_UNIX_11	1		/* UNIX 11 */
#define LIST_UNIX_12	2		/* UNIX 12 */
#define LIST_UNIX_13	3		/* UNIX 13 */
#define LIST_UNIX_14	4		/* UNIX 14 */
#define LIST_UNIX_15	5		/* UNIX 15 */
#define LIST_UNIX_20	6		/* UNIX 20 */
#define LIST_UNIX_21	7		/* UNIX 21 */
#define LIST_UNIX_22	8		/* UNIX 22 */
#define LIST_UNIX_23	9		/* UNIX 23 */
#define LIST_UNIX_24	10		/* UNIX 24 */
#define LIST_UNIX_25	11		/* UNIX 25 */
#define LIST_UNIX_50	12		/* UNIX 50 */
#define LIST_UNIX_51	13		/* UNIX 51 */
#define LIST_UNIX_54	14		/* UNIX 54 */
#define LIST_UNIX_60	15		/* UNIX 60 */
#define LIST_UNIX_61	16		/* UNIX 61 */
#define LIST_UNIX_62	17		/* UNIX 62 */
#define LIST_UNIX_63	18		/* UNIX 63 */
#define LIST_UNIX_64	19		/* UNIX 64 */
#define LIST_UNIX_65	20		/* UNIX 65 */
#define LIST_DOS_1		21		/* MS-DOS 1 */
#define LIST_DOS_2		22		/* MS-DOS 2 */
#define LIST_DOS_3		23		/* MS-DOS 3 */
#define LIST_DOS_4		24		/* MS-DOS 4 */
#define LIST_ACOS		25		/* ACOS */
#define LIST_AS400		26		/* AS/400 */
#define LIST_M1800		27		/* Fujitu M1800 (OS IV/MSP E20) */
#define LIST_CHAMELEON	28		/* Win3.1�p Chameleon FTP server */
#define LIST_GP6000		29		/* Fujitu GP6000 Model 900 */
#define LIST_OS2		30		/* OS/2 */
#define LIST_VMS		31		/* VAX VMS */
#define LIST_OS7_1		32		/* Toshiba OS7 */
#define LIST_OS7_2		33		/* Toshiba OS7 */
#define LIST_IRMX		34		/* IRMX */
#define LIST_ACOS_4		35		/* ACOS-4 */
#define LIST_STRATUS	36		/* Stratus */
#define LIST_ALLIED		37		/* allied telesis (DOS) */
#define LIST_OS9		38		/* OS/9 */
#define LIST_IBM		39		/* IBM host */
#define LIST_AGILENT	40		/* Agilent logic analyzer */
#define LIST_SHIBASOKU	41		/* Shibasoku LSI test system */
#define LIST_UNIX_70	42		/* UNIX 70 */
#define LIST_UNIX_71	43		/* UNIX 71 */
#define LIST_UNIX_72	44		/* UNIX 72 */
#define LIST_UNIX_73	45		/* UNIX 73 */
#define LIST_UNIX_74	46		/* UNIX 74 */
#define LIST_UNIX_75	47		/* UNIX 75 */

#define LIST_MELCOM		0x100	/* MELCOM80 */

#define LIST_MASKFLG	0xFF

/* �t�@�C���ꗗ���� ---------------

*LIST_UNIX_10
	0          1   2     3      4    5    6   7         8
	-------------------------------------------------------
	drwxr-xr-x 15  owner group  1024 Nov  6   14:21     Linux/
	-rwxrwx---  5  owner group    12 Nov  6   1996      test.txt
	drwxr-xr-x 15  owner group  1024 11�� 6�� 14:21     Linux/
	drwxr-xr-x 15  owner group  1024 11�� 6�� 14��21��  Linux/
	-rwxrwx---  5  owner group    12 11�� 6�� 1996�N    test.txt
	drwxrwxr-x 6   root  sys     512  1�� 26  03:10     adm		(����GB�R�[�h��0xD4C2)

*LIST_UNIX_11
	0          1   2     3      4    5        6         7
	-------------------------------------------------------
	drwxr-xr-x 15  owner group  1024 11��12�� 14��21��  Linux/
	-rwxrwx---  5  owner group    12 11��12�� 1996�N    test.txt

*LIST_UNIX_12
	0              1     2      3    4    5   6         7
	-------------------------------------------------------
	drwxr-xr-x123  owner group  1024 Nov  6   14:21     Linux/
	-rwxrwx---132  owner group    12 Nov  6   1996      test.txt
	drwxr-xr-x123  owner group  1024 11�� 6�� 14:21     Linux/
	drwxr-xr-x123  owner group  1024 11�� 6�� 14��21��  Linux/
	-rwxrwx---132  owner group    12 11�� 6�� 1996�N    test.txt

*LIST_UNIX_13
	0              1     2      3    4        5         6
	-------------------------------------------------------
	drwxr-xr-x123  owner group  1024 11��12�� 14��21��  Linux/
	-rwxrwx---132  owner group    12 11��12�� 1996�N    test.txt

*LIST_UNIX_14
	0          1   2     3      4    5    6   7         8
	-------------------------------------------------------
	drwxr-xr-x 15  owner group  512  2001 6�� 18        audit	(����GB�R�[�h��0xD4C2)

*LIST_UNIX_15
	0              1     2      3    4    5   6         7
	-------------------------------------------------------
	drwxr-xr-x15   owner group  512  2001 6�� 18        audit	(����GB�R�[�h��0xD4C2)





*LIST_UNIX_20
	0          1   2            3    4    5   6         7
	-------------------------------------------------------
	drwxr-xr-x 15  owner        1024 Nov  6   14:21     Linux/
	-rwxrwx---  5  owner          12 Nov  6   1996      test.txt
	drwxr-xr-x 15  owner        1024 11�� 6�� 14:21     Linux/
	drwxr-xr-x 15  owner        1024 11�� 6�� 14��21��  Linux/
	-rwxrwx---  5  owner          12 11�� 6�� 1996�N    test.txt

*LIST_UNIX_21
	0          1   2            3    4        5         6
	-------------------------------------------------------
	drwxr-xr-x 15  owner        1024 11��12�� 14��21��  Linux/
	-rwxrwx---  5  owner          12 11��12�� 1996�N    test.txt

*LIST_UNIX_22
	0              1            2    3    4   5         6
	-------------------------------------------------------
	drwxr-xr-x123  owner        1024 Nov  6   14:21     Linux/
	-rwxrwx---132  owner          12 Nov  6   1996      test.txt
	drwxr-xr-x123  owner        1024 11�� 6�� 14:21     Linux/
	drwxr-xr-x123  owner        1024 11�� 6�� 14��21��  Linux/
	-rwxrwx---132  owner          12 11�� 6�� 1996�N    test.txt

*LIST_UNIX_23
	0              1            2    3        4         5
	-------------------------------------------------------
	drwxr-xr-x123  owner        1024 11��12�� 14��21��  Linux/
	-rwxrwx---132  owner          12 11��12�� 1996�N    test.txt

*LIST_UNIX_24
	0          1   2            3    4    5   6         7
	-------------------------------------------------------
	drwxr-xr-x 15  owner        512  2001 6�� 18        audit	(����GB�R�[�h��0xD4C2)

*LIST_UNIX_25
	0              1            2    3    4   5         6
	-------------------------------------------------------
	drwxr-xr-x15   owner        512  2001 6�� 18        audit	(����GB�R�[�h��0xD4C2)







*LIST_UNIX_50
	0              1            2    3    4   5         6
	-------------------------------------------------------
	drwxr-xr-x     owner        1024 Nov  6   14:21     Linux/
	-rwxrwx---     owner          12 Nov  6   1996      test.txt
	drwxr-xr-x     owner        1024 11�� 6�� 14:21     Linux/
	drwxr-xr-x     owner        1024 11�� 6�� 14��21��  Linux/
	-rwxrwx---     owner          12 11�� 6�� 1996�N    test.txt

*LIST_UNIX_51
	0              1            2    3        4         5
	-------------------------------------------------------
	drwxr-xr-x     owner        1024 11��12�� 14��21��  Linux/
	-rwxrwx---     owner          12 11��12�� 1996�N    test.txt

	0          1   2        3        4        5
	-------------------------------------------------------
    -rwxrwxrwx SEQ 36203776 01/07/07 12:38:28 ADRS001                         
    -rwxrwxrwx SEQ 70172160 01/07/07 13:59:58 ADRS002                         

*LIST_UNIX_54
	0              1            2    3    4   5         6
	-------------------------------------------------------
	drwxr-xr-x     owner        512  2001 6�� 18        audit	(����GB�R�[�h��0xD4C2)







*LIST_UNIX_60
	0          1    2     3 4     5 6    7    8  9     10
	-------------------------------------------------------
	drwxr-xr-x 123  owner m group g 1024 Nov  6  14:21 Linux/
	-rwxrwx--- 132  owner m group g   12 Nov  6  1996  test.txt

*LIST_UNIX_61
	0          1    2     3 4     5 6    7         8     9
	-------------------------------------------------------
	drwxr-xr-x 123  owner m group g 1024 11��12��  14:21 Linux/
	-rwxrwx--- 132  owner m group g   12 11��12��  1996  test.txt

*LIST_UNIX_62
	0              1     2 3     4 5    6    7  8     9
	-------------------------------------------------------
	drwxr-xr-x123  owner m group g 1024 Nov  6  14:21 Linux/
	-rwxrwx---132  owner m group g   12 Nov  6  1996  test.txt

*LIST_UNIX_63
	0              1     2 3     4 5    6         7     8
	-------------------------------------------------------
	drwxr-xr-x123  owner m group g 1024 11��12��  14:21 Linux/
	-rwxrwx---132  owner m group g   12 11��12��  1996  test.txt

*LIST_UNIX_64
	0          1   2     3 4     5  6    7    8   9    10
	-------------------------------------------------------
	drwxr-xr-x 15  owner m group g  512  2001 6�� 18   audit	(����GB�R�[�h��0xD4C2)

*LIST_UNIX_65
	0              1     2 3     4  5    6    7   8    9
	-------------------------------------------------------
	drwxr-xr-x15   owner m group g  512  2001 6�� 18   audit	(����GB�R�[�h��0xD4C2)




LIST_UNIX_70
	0          1    2       3     4 5    6    7  8     9
	-------------------------------------------------------
	drwxr-xr-x 123  owner   group g 1024 Nov  6  14:21 Linux/
	-rwxrwx--- 132  owner   group g   12 Nov  6  1996  test.txt

*LIST_UNIX_71
	0          1    2       3     4 5    6         7     8
	-------------------------------------------------------
	drwxr-xr-x 123  owner   group g 1024 11��12��  14:21 Linux/
	-rwxrwx--- 132  owner   group g   12 11��12��  1996  test.txt

*LIST_UNIX_72
	0              1       2     3 4    5    6  7     8
	-------------------------------------------------------
	drwxr-xr-x123  owner   group g 1024 Nov  6  14:21 Linux/
	-rwxrwx---132  owner   group g   12 Nov  6  1996  test.txt

*LIST_UNIX_73
	0              1       2     3 4    5         6     7
	-------------------------------------------------------
	drwxr-xr-x123  owner   group g 1024 11��12��  14:21 Linux/
	-rwxrwx---132  owner   group g   12 11��12��  1996  test.txt

*LIST_UNIX_74
	0          1   2       3     4  5    6    7   8    9
	-------------------------------------------------------
	drwxr-xr-x 15  owner   group g  512  2001 6�� 18   audit	(����GB�R�[�h��0xD4C2)

*LIST_UNIX_75
	0              1       2     3  4    5    6   7    8
	-------------------------------------------------------
	drwxr-xr-x15   owner   group g  512  2001 6�� 18   audit	(����GB�R�[�h��0xD4C2)






*unix�n�ňȉ��̂悤�ȓ��t
	0              1            2    3   4    5         6
	-------------------------------------------------------
	drwxr-xr-x123  owner        1024 11/ 6    14:21     Linux/
	-rwxrwx---132  owner          12 11/13    1996      test.txt


















*LIST_DOS_1
	0         1          2       3
	-------------------------------------------------------
	97-10-14  03:34p     <DIR>   Linux
	97-10-14  03:34p        12   test.txt
	100-10-14 03:34p        12   test.txt

*LIST_DOS_2
	0         1          2       3
	-------------------------------------------------------
	10-14-97  03:34p     <DIR>   Linux
	10-14-97  03:34p        12   test.txt
	10-14-100 03:34p        12   test.txt

*LIST_DOS_3
	0             1      2         3       4
	-------------------------------------------------------
	Linux         <DIR>  10-14-97  03:34    
	test.txt         12  10-14-97  14:34   A
	test.txt         12  10-14-100 14:34   A

*LIST_DOS_4
	0          1            2        3
	-------------------------------------------------------
    1998/07/30 15:39:02     <DIR>    Linux
    1998/07/30 15:42:19     11623    test.txt

*LIST_ACOS
	0
	-------------------------------------------------------
	test.txt
�@�@�f�B���N�g���Ȃ��A

*LIST_AS400
	0           1     2        3        4        5
	-------------------------------------------------------
	QSYS        18944 96/09/20 00:35:10 *DIR     QOpenSys/
	QDOC        26624 70/01/01 00:00:00 *FLR     QDLS/
	QSYS            0 98/09/27 10:00:04 *LIB     QSYS.LIB/
	QSECOFR         0 98/05/15 16:01:15 *STMF    WWWTEST.BAK

*LIST_M1800
	0     1     2       3       4     5         6 (�t�@�C�����̌��ɃX�y�[�X����j
	-------------------------------------------------------
	drwx  F        400     400  PO    93.10.27  COMMON.PDL.EXCEL/       
	-rw-  F      10000   10000  DA    97.03.04  DTSLOG1.FNA             
	-rw-  F      10000  ******  DA    97.03.04  DTSBRB.FNA              
	drwx  U     ******    6144  PO    96.12.15  IS01.TISPLOAD/          
	-rw-  ****  ******  ******  VSAM  **.**.**  HICS.CMDSEQ             

*LIST_CHAMELEON
	0            1        2    3 4    5     6
	-------------------------------------------------------
    Linux        <DIR>    Nov  6 1997 14:21 drw-
    test.txt           12 Nov  6 1886 14:21 -rwa

*LIST_GP6000
	0          1        2        3        4        5    6
	-------------------------------------------------------
	drwxrwxrwx 98.10.21 14:38:46 SYSG03   XSYSOPR  2048 atlib
	-rwxrwxrwx 97.10.30 11:06:04 XSYSMNGR XSYSOPR  2048 blib

*LIST_OS2
	   0        1          2          3      4
	-------------------------------------------------------
       345      A          12-02-98   10:59  VirtualDevice.java
         0           DIR   12-09-98   09:43  �f�B���N�g��
         0           DIR   12-09-100  09:43  �f�B���N�g��

*LIST_MELCOM
	0 1           2   3          4  5    6  7    8
	---------------------------------------------------------------
    - RW-RW-RW-   1   TERA       50 DEC  1  1997 AAAJ          B(B)
    - RW-RW-RW-   1   TERA        1 AUG  7  1998 12345678901234B(B)
    d RWXRWXRWX   2   TERA       64 NOV 13  1997 Q2000         -

*LIST_VMS
	0                  1         2           3         4
	---------------------------------------------------------------
	CIM_ALL.MEM;5        2/4     21-APR-1998 11:01:17  [CIM,MIZOTE]
	(RWED,RWED,RE,)
	MAIL.DIR;1         104/248   18-SEP-2001 16:19:39  [CIM,MIZOTE]
	(RWE,RWE,,)
		��VMS�̏ꍇ�ꗗ�������s�ɕʂ��ꍇ������

*LIST_OS7_1
	0                       1        2        3
	---------------------------------------------------------------
    drwxrwxrwx              99/05/13 11:38:34 APL
*LIST_OS7_2
	0          1      2     3        4        5
	---------------------------------------------------------------
    -rwxrwxrwx SEQ    17408 96/12/06 10:11:27 INIT_CONFIG

*LIST_IRMX
	0          1   2     3  4       5       6 7 8         9  10  11
	---------------------------------------------------------------
    world      DR  DLAC  1    416   1,024   1 WORLD       05 FEB 98
    world      DR        1    416   1,024   1 WORLD       05 FEB 98
    name.f38       DRAU  5  4,692   1,024   1 # 0         24 MAR 99
    name.f38             5  4,692   1,024   1 # 0         24 MAR 99

*LIST_STRATUS
	 0      1  2         3        4         5
	---------------------------------------------------------------
	Files: 15  Blocks: 29
	 w      1  seq       99-06-15 13:11:39  member_srv.error
	Dirs: 74
	 m      3  98-12-25 16:14:58  amano

*LIST_ALLIED
	 0             1        2   3   4  5        6
	---------------------------------------------------------------
	     41622     IO.SYS   Tue Dec 20 06:20:00 1994
	<dir>             DOS   Wed Nov 24 09:35:48 1999

*LIST_OS9
	 0       1        2     3            4      5      6
	---------------------------------------------------------------
     0.0     01/02/13 0945  d-----wr     3C0    148724 W_017
     0.0     01/02/13 0945  ------wr     C20     48828 W_017.CLG

*LIST_IBM
	 0      1      2           3  4    5      6   7      8   9
	---------------------------------------------------------------
     JXSIB1 3390   2000/12/27  1  810  FB     240 24000  PO  DIRNAME
     JXSW01 3390   2000/12/27  1    5  VBA    240  3120  PS  FILENAME

*LIST_AGILENT
	 0             1    2    3      4     5
	---------------------------------------------------------------
     drwxrwxrwx    1    1    1      1024  system
     -rw-rw-rw-    1    1    1      1792  abc.aaa

*LIST_SHIBASOKU
	 0        1            2          3                 4
	---------------------------------------------------------------
       512    Jan-30-2002  14:52:04   DIRNAME           <DIR>
     61191    Aug-30-2002  17:30:38   FILENAME.C        


------------------------------------*/

/*===== �ڑ��E�C���h�E�̌`�� =====*/

#define DLG_TYPE_CON	0		/* �ȈՁi�z�X�g�ҏW�Ȃ��j */
#define DLG_TYPE_SET	1		/* �z�X�g�ҏW���� */

/*===== �t�@�C���ꗗ�擾���[�h =====*/

#define CACHE_NORMAL	0		/* �ʏ�i�L���b�V���ɂ���Ύg�p�A�Ȃ���Γǂݍ��݁j */
#define CACHE_REFRESH	1		/* ��ɓǂݍ��� */
#define CACHE_LASTREAD	2		/* �Ō�ɓǂݍ��񂾂��̂��g�p */

/*===== �����R�[�h�ϊ� =====*/

#define KANJI_SJIS		0		/* SJIS */
#define KANJI_JIS		1		/* JIS */
#define KANJI_EUC		2		/* EUC */
#define KANJI_SMB_HEX	3		/* Samba-HEX */
#define KANJI_SMB_CAP	4		/* Samba-CAP */
#define KANJI_UTF8N		5		/* UTF-8N */

#define KANJI_NOCNV		0		/* �����R�[�h�ϊ��Ȃ� */

/*===== �T�E���h =====*/

#define SND_CONNECT		0		/* �ڑ����̃T�E���h */
#define SND_TRANS		1		/* �]���I�����̃T�E���h */
#define SND_ERROR		2		/* �G���[���̃T�E���h */

#define SOUND_TYPES		3		/* �T�E���h�̎�� */

/*===== �r���[�� =====*/

#define VIEWERS			3		/* �r���[���̐� */

/*===== �u�b�N�}�[�N =====*/

#define BMARK_SUB_MENU		2		/* �u�b�N�}�[�N���j���[�̃T�u���j���[�ԍ� */
#define DEFAULT_BMARK_ITEM	5		/* �u�b�N�}�[�N���j���[�ɂ���Œ蕔���̐� */
#define MENU_BMARK_TOP		30000	/* 3000�ȍ~(3100���炢�܂�)�͗\�񂷂� */
									/* resource.h �̒�`�Əd�Ȃ�Ȃ��悤�� */

#define BMARK_TYPE_NONE		0		/* �u�b�N�}�[�N���� */
#define BMARK_TYPE_LOCAL	1		/* ���[�J�����̃u�b�N�}�[�N */
#define BMARK_TYPE_REMOTE	2		/* �z�X�g���̃u�b�N�}�[�N */
#define BMARK_TYPE_BOTH		3		/* �����̃u�b�N�}�[�N */

#define BMARK_MARK_LOCAL	"L "	/* ���[�J�����̈� */
#define BMARK_MARK_REMOTE	"H "	/* �z�X�g���̈� */
#define BMARK_MARK_BOTH		"W "	/* �����̈� */
#define BMARK_MARK_LEN		2		/* ��̕����� */

#define BMARK_SEP			" <> "	/* ���[�J�����ƃz�X�g���̋�؂� */
#define BMARK_SEP_LEN		4		/* ��؂�̕����� */

/*===== ���W�X�g���̃^�C�v =====*/

#define REGTYPE_REG		0		/* ���W�X�g�� */
#define REGTYPE_INI		1		/* INI�t�@�C�� */

#define REG_SECT_MAX	(16*1024)	/* ���W�X�g���̂P�Z�N�V�����̍ő�f�[�^�T�C�Y */

/*===== �z�X�g�ݒ�Ŗ����I�Ɏw�肷��z�X�g�̃^�C�v =====*/

#define	HTYPE_AUTO		0		/* ���� */
#define	HTYPE_ACOS		1		/* ACOS (�ҋ@�����t�@�C���̎w�肪�K�v) */
#define	HTYPE_VMS		2		/* VAX VMS */
#define	HTYPE_IRMX		3		/* IRMX */
#define	HTYPE_ACOS_4	4		/* ACOS �t�@�C������('')�Ŋ���Ȃ� */
#define	HTYPE_STRATUS	5		/* Stratus */
#define	HTYPE_AGILENT	6		/* Agilent Logic analyzer */
#define	HTYPE_SHIBASOKU	7		/* Shibasoku LSI test system */

/*===== �R�}���h���C���I�v�V���� =====*/

#define OPT_MIRROR		0x0001	/* �~���[�����O�A�b�v���[�h���s�� */
#define OPT_FORCE		0x0002	/* �~���[�����O�J�n�̊m�F�����Ȃ� */
#define OPT_QUIT		0x0004	/* �I����v���O�����I�� */
#define OPT_EUC			0x0008	/* �����R�[�h��EUC */
#define OPT_JIS			0x0010	/* �����R�[�h��JIS */
#define OPT_ASCII		0x0020	/* �A�X�L�[�]�����[�h */
#define OPT_BINARY		0x0040	/* �o�C�i���]�����[�h */
#define OPT_AUTO		0x0080	/* �������� */
#define OPT_KANA		0x0100	/* ���p���Ȃ����̂܂ܒʂ� */
#define OPT_EUC_NAME	0x0200	/* �t�@�C������EUC */
#define OPT_JIS_NAME	0x0400	/* �t�@�C������JIS */
#define OPT_MIRRORDOWN	0x0800	/* �~���[�����O�_�E�����[�h���s�� */
#define OPT_SAVEOFF		0x1000	/* �ݒ�̕ۑ��𒆎~���� */
#define OPT_SAVEON		0x2000	/* �ݒ�̕ۑ����ĊJ���� */

/*===== �z�X�g�̃q�X�g�� =====*/

#define	HISTORY_MAX		20		/* �t�@�C���̃q�X�g���̍ő�� */
#define DEF_FMENU_ITEMS	8		/* File���j���[�ɂ��鍀�ڐ��̏����l */

/*===== SOCKS4 =====*/

#define SOCKS4_VER			4	/* SOCKS�̃o�[�W���� */

#define SOCKS4_CMD_CONNECT	1	/* CONNECT�R�}���h */
#define SOCKS4_CMD_BIND		2	/* BIND�R�}���h */

/* ���U���g�R�[�h */
#define SOCKS4_RES_OK		90	/* �v���͋����ꂽ */
	/* ���̑��̃R�[�h�̓`�F�b�N���Ȃ��̂Œ�`���Ȃ� */

/*===== SOCKS5 =====*/

#define SOCKS5_VER			5	/* SOCKS�̃o�[�W���� */

#define SOCKS5_CMD_CONNECT	1	/* CONNECT�R�}���h */
#define SOCKS5_CMD_BIND		2	/* BIND�R�}���h */

#define SOCKS5_AUTH_NONE	0	/* �F�ؖ��� */
#define SOCKS5_AUTH_GSSAPI	1	/* GSS-API */
#define SOCKS5_AUTH_USER	2	/* Username/Password */

#define SOCKS5_ADRS_IPV4	1	/* IP V4 address */
#define SOCKS5_ADRS_NAME	3	/* Domain name */
#define SOCKS5_ADRS_IPV6	4	/* IP V6 address */

#define SOCKS5_USERAUTH_VER	1	/* Username\Password�F�؂̃o�[�W���� */

/* ���U���g�R�[�h */
#define SOCKS5_RES_OK		0x00	/* succeeded */
	/* ���̑��̃R�[�h�̓`�F�b�N���Ȃ��̂Œ�`���Ȃ� */

/*===== ���f�R�[�h =====*/

#define ABORT_NONE			0		/* �]�����f�Ȃ� */
#define ABORT_USER			1		/* ���[�U�ɂ�钆�f */
#define ABORT_ERROR			2		/* �G���[�ɂ�钆�f */
#define ABORT_DISKFULL		3		/* �f�B�X�N�t����钆�f */

/*===== FireWall�̎�� =====*/

#define FWALL_NONE			0		/* �Ȃ� */
#define FWALL_FU_FP_SITE	1		/* FW user �� FW pass �� SITE host */
#define FWALL_FU_FP_USER	2		/* FW user �� FW pass �� USER user@host */
#define FWALL_USER			3		/* USER user@host */
#define FWALL_OPEN			4		/* OPEN host */
#define FWALL_SOCKS4		5		/* SOCKS4 */
#define FWALL_SOCKS5_NOAUTH	6		/* SOCKS5 (�F�؂Ȃ�) */
#define FWALL_SOCKS5_USER	7		/* SOCKS5 (Username/Password�F��) */
#define FWALL_FU_FP			8		/* FW user �� FW pass */
#define FWALL_SIDEWINDER	9		/* USER FWuser:FWpass@host */

/*===== �����^�C���p�X���[�h =====*/

/* �R�[�h�̎�� */
#define SECURITY_DISABLE	0		/* �g�p���Ȃ� */
#define SECURITY_AUTO		1		/* �����F�� */
#define MD4					2		/* MD4 */
#define MD5					3		/* MD5 */
#define SHA1				4		/* SHA-1 */

#define MAX_SEED_LEN		16		/* Seed�̕����� */

/*===== �ċA�I�ȃt�@�C�������̕��@ =====*/

#define RDIR_NONE		0		/* �ċA�����Ȃ� */
#define RDIR_NLST		1		/* NLST -R */
#define RDIR_CWD		2		/* CWD�ňړ� */

/*===== VAX VMS �֌W =====*/

#define BLOCK_SIZE		512		/* 1�u���b�N�̃o�C�g�� */

/*===== �ݒ�̃��x�� =====*/

#define SET_LEVEL_GROUP		0x8000		/* �O���[�v�̃t���O */
#define SET_LEVEL_MASK		0x7FFF
#define SET_LEVEL_SAME		0x7FFF

/*===== ���t�A���l�̕\���`�� =====*/

#define DISPFORM_LEGACY		0		/* �Œ菑�� */
#define DISPFORM_LOCALE		1		/* �R���g���[���p�l���ɏ]�� */

/*===== �t�@�C���ꗗ�ɑ��݂������ =====*/

#define FINFO_DATE			0x01	/* ���t */
#define FINFO_TIME			0x02	/* ���� */
#define FINFO_SIZE			0x04	/* �T�C�Y */
#define FINFO_ATTR			0x08	/* ���� */
#define FINFO_ALL			0xFF	/* �S�� */

/*===== GetMasterPasswordStatus�Ŏg���R�[�h =====*/
#define PASSWORD_OK 0
#define PASSWORD_UNMATCH 2
#define BAD_PASSWORD_HASH 3

/*===== �Í����p�X���[�h�ݒ� =====*/
#define DEFAULT_PASSWORD	"DefaultPassword"
#define MAX_PASSWORD_LEN	128


/*=================================================
*		�X�g���N�`��
*=================================================*/

/*===== �z�X�g�ݒ�f�[�^ =====*/

typedef struct {
	int Level;							/* �ݒ�̃��x�� */
										/* �ʏ�̓O���[�v�̃t���O�݂̂��L�� */
										/* ���x�����͐ݒ�̓o�^�^�ďo���݂̂Ŏg�p */
	char HostName[HOST_NAME_LEN+1];		/* �ݒ薼 */
	char HostAdrs[HOST_ADRS_LEN+1];		/* �z�X�g�� */
	char UserName[USER_NAME_LEN+1];		/* ���[�U�� */
	char PassWord[PASSWORD_LEN+1];		/* �p�X���[�h */
	char Account[ACCOUNT_LEN+1];		/* �A�J�E���g */
	char LocalInitDir[INIT_DIR_LEN+1];	/* ���[�J���̊J�n�f�B���N�g�� */
	char RemoteInitDir[INIT_DIR_LEN+1];	/* �z�X�g�̊J�n�f�B���N�g�� */
	char BookMark[BOOKMARK_SIZE];		/* �u�b�N�}�[�N */
	char ChmodCmd[CHMOD_CMD_LEN+1];		/* �����ύX�R�}���h */
	char LsName[NLST_NAME_LEN+1];		/* NLST�ɕt����t�@�C����/�I�v�V����*/
	char InitCmd[INITCMD_LEN+1];		/* �z�X�g�̏������R�}���h */
	int Port;							/* �|�[�g�ԍ� */
	int Anonymous;						/* Anonymous�t���O */
	int KanjiCode;						/* �z�X�g�̊����R�[�h (KANJI_xxx) */
	int KanaCnv;						/* ���p�J�i��S�p�ɕϊ�(YES/NO) */
	int NameKanjiCode;					/* �t�@�C�����̊����R�[�h (KANJI_xxx) */
	int NameKanaCnv;					/* �t�@�C�����̔��p�J�i��S�p�ɕϊ�(YES/NO) */
	int Pasv;							/* PASV���[�h (YES/NO) */
	int FireWall;						/* FireWall���g�� (YES/NO) */
	int ListCmdOnly;					/* "LIST"�R�}���h�̂ݎg�p���� */
	int UseNLST_R;						/* "NLST -R"�R�}���h���g�p���� */
	int LastDir;						/* �Ō�ɃA�N�Z�X�����t�H���_��ۑ� */
	int TimeZone;						/* �^�C���]�[�� (-12�`12) */
	int HostType;						/* �z�X�g�̃^�C�v (HTYPE_xxx) */
	int SyncMove;						/* �t�H���_�����ړ� (YES/NO) */
	int NoFullPath;						/* �t���p�X�Ńt�@�C���A�N�Z�X���Ȃ� (YES/NO) */
	ulong Sort;							/* �\�[�g���@ (0x11223344 : 11=LFsort 22=LDsort 33=RFsort 44=RFsort) */
	int Security;						/* �Z�L�����e�B (SECURITY_xxx , MDx) */
	int Dialup;							/* �_�C�A���A�b�v�ڑ����邩�ǂ��� (YES/NO) */
	int DialupAlways;					/* ��ɂ��̃G���g���֐ڑ����邩�ǂ��� (YES/NO) */
	int DialupNotify;					/* �Đڑ��̍ۂɊm�F���� (YES/NO) */
	char DialEntry[RAS_NAME_LEN+1];		/* �_�C�A���A�b�v�G���g�� */
} HOSTDATA;


/*===== �z�X�g�ݒ胊�X�g =====*/

typedef struct hostlistdata {
	HOSTDATA Set;					/* �z�X�g�ݒ�f�[�^ */
	struct hostlistdata *Next;
	struct hostlistdata *Prev;
	struct hostlistdata *Child;
	struct hostlistdata *Parent;
} HOSTLISTDATA;


/*===== �ڑ��q�X�g�����X�g =====*/

typedef struct historydata {
	char HostAdrs[HOST_ADRS_LEN+1];		/* �z�X�g�� */
	char UserName[USER_NAME_LEN+1];		/* ���[�U�� */
	char PassWord[PASSWORD_LEN+1];		/* �p�X���[�h */
	char Account[ACCOUNT_LEN+1];		/* �A�J�E���g */
	char LocalInitDir[INIT_DIR_LEN+1];	/* �f�B���N�g�� */
	char RemoteInitDir[INIT_DIR_LEN+1];	/* �f�B���N�g�� */
	char ChmodCmd[CHMOD_CMD_LEN+1];		/* �����ύX�R�}���h */
	char LsName[NLST_NAME_LEN+1];		/* NLST�ɕt����t�@�C����/�I�v�V����*/
	char InitCmd[INITCMD_LEN+1];		/* �z�X�g�̏������R�}���h */
	int Port;							/* �|�[�g�ԍ� */
	int KanjiCode;						/* �z�X�g�̊����R�[�h (KANJI_xxx) */
	int KanaCnv;						/* ���p�J�i��S�p�ɕϊ�(YES/NO) */
	int NameKanjiCode;					/* �t�@�C�����̊����R�[�h (KANJI_xxx) */
	int NameKanaCnv;					/* �t�@�C�����̔��p�J�i��S�p�ɕϊ�(YES/NO) */
	int Pasv;							/* PASV���[�h (YES/NO) */
	int FireWall;						/* FireWall���g�� (YES/NO) */
	int ListCmdOnly;					/* "LIST"�R�}���h�̂ݎg�p���� */
	int UseNLST_R;						/* "NLST -R"�R�}���h���g�p���� */
	int TimeZone;						/* �^�C���]�[�� (-12�`12) */
	int HostType;						/* �z�X�g�̃^�C�v (HTYPE_xxx) */
	int SyncMove;						/* �t�H���_�����ړ� (YES/NO) */
	int NoFullPath;						/* �t���p�X�Ńt�@�C���A�N�Z�X���Ȃ� (YES/NO) */
	ulong Sort;							/* �\�[�g���@ (0x11223344 : 11=LFsort 22=LDsort 33=RFsort 44=RFsort) */
	int Security;						/* �Z�L�����e�B (OTP_xxx , MDx) */
	int Type;							/* �]�����@ (TYPE_xx) */
	int Dialup;							/* �_�C�A���A�b�v�ڑ����邩�ǂ��� (YES/NO) */
	int DialupAlways;					/* ��ɂ��̃G���g���֐ڑ����邩�ǂ��� (YES/NO) */
	int DialupNotify;					/* �Đڑ��̍ۂɊm�F���� (YES/NO) */
	char DialEntry[RAS_NAME_LEN+1];		/* �_�C�A���A�b�v�G���g�� */
	struct historydata *Next;
} HISTORYDATA;


/*===== �]���t�@�C�����X�g =====*/

typedef struct transpacket {
	SOCKET ctrl_skt;				/* Socket */
	char Cmd[40];					/* STOR/RETR/MKD */
	char RemoteFile[FMAX_PATH+1];	/* �z�X�g���̃t�@�C�����i�t���p�X�j */
									/* VMS�̎��� ddd[xxx.yyy]/yyy/zzz �̂悤�� */
									/* �Ȃ��Ă�̂Œ��� */
	char LocalFile[FMAX_PATH+1];	/* ���[�J�����̃t�@�C�����i�t���p�X�j */
	int Type;						/* �]�����@ (TYPE_xx) */
	LONGLONG Size;					/* �t�@�C���̃T�C�Y */
	LONGLONG ExistSize;				/* ���łɑ��݂���t�@�C���̃T�C�Y */
									/* �]�����́A�]�������t�@�C���̃T�C�Y���i�[���� */
	FILETIME Time;					/* �t�@�C���̎���(UTC) */
	int Attr;						/* �t�@�C���̑��� */
	int KanjiCode;					/* �����R�[�h (KANJI_xxx) */
	int KanaCnv;					/* ���p�J�i��S�p�ɕϊ�(YES/NO) */
	int Mode;						/* �]�����[�h (EXIST_xxx) */
	HWND hWndTrans;					/* �]�����_�C�A���O�̃E�C���h�E�n���h�� */
	int Abort;						/* �]�����~�t���O (ABORT_xxx) */
	struct transpacket *Next;
} TRANSPACKET;


/*===== �t�@�C�����X�g =====*/

typedef struct filelist {
	char File[FMAX_PATH+1];			/* �t�@�C���� */
	char Node;						/* ��� (NODE_xxx) */
	char Link;						/* �����N�t�@�C�����ǂ��� (YES/NO) */
	LONGLONG Size;					/* �t�@�C���T�C�Y */
	int Attr;						/* ���� */
	FILETIME Time;					/* ����(UTC) */
	char Owner[OWNER_NAME_LEN+1];	/* �I�[�i�� */
	char InfoExist;					/* �t�@�C���ꗗ�ɑ��݂������̃t���O (FINFO_xxx) */
	struct filelist *Next;
} FILELIST;


/*===== �R�[�h�ϊ����p�P�b�g =====*/

typedef char * (*funcptr)(struct codeconvinfo *, char , char *);

typedef struct codeconvinfo {
	char *Str;			/* ������ */
	int StrLen;			/* ������̒��� */
	int KanaCnv;		/* ���p�J�^�J�i��S�p�ɕϊ����邩�ǂ��� (YES/NO) */
	char *Buf;			/* �ϊ���̕�������i�[����o�b�t�@ */
	int BufSize;		/* �ϊ���̕�������i�[����o�b�t�@�̃T�C�Y */
	int OutLen;			/* �ϊ���̕�����̃T�C�Y */
	int KanjiMode;		/* �������[�h�t���O(YES/NO) (���������p���[�N) */
	int EscProc;		/* �G�X�P�[�v�V�[�P���X������ (0�`) (���������p���[�N) */
	char EscCode[2];	/* �G�X�P�[�v�V�[�P���X�����ۑ��p (���������p���[�N) */
	char KanjiFst;		/* �����R�[�h�P�o�C�g�ڕۑ��p (���������p���[�N) */
	char KanaPrev;		/* ���p�J�^�J�i�ۑ��p (���������p���[�N) */
	funcptr KanaProc;	/* ���p�J�^�J�i�������[�`�� (���������p���[�N) */
} CODECONVINFO;


/*===== ���s�R�[�h�ϊ����p�P�b�g =====*/

typedef struct termcodeconvinfo {
	char *Str;			/* ������ */
	int StrLen;			/* ������̒��� */
	char *Buf;			/* �ϊ���̕�������i�[����o�b�t�@ */
	int BufSize;		/* �ϊ���̕�������i�[����o�b�t�@�̃T�C�Y */
	int OutLen;			/* �ϊ���̕�����̃T�C�Y */
	char Term;			/* ���s�R�[�h�P�o�C�g�ڕۑ��p (���������p���[�N) */
} TERMCODECONVINFO;


/*===== �e���|�����t�@�C�����X�g =====*/

typedef struct tempfilelist {
	char *Fname;				/* �t�@�C���� */
	struct tempfilelist *Next;
} TEMPFILELIST;


/*===== �T�E���h�t�@�C�� =====*/

typedef struct {
	int On;						/* ON/OFF�X�C�b�` */
	char Fname[FMAX_PATH+1];		/* �t�@�C���� */
} SOUNDFILE;


/*===== ���W�I�{�^���̐ݒ� =====*/

typedef struct {
	int ButID;			/* �{�^����ID */
	int Value;			/* �l */
} RADIOBUTTON;


/*===== SOCKS4 =====*/

/* �R�}���h�p�P�b�g */
typedef struct {
	char Ver;						/* �o�[�W���� (SOCKS4_VER) */
	char Cmd;						/* �R�}���h (SOCKS4_CMD_xxx) */
	ushort Port;					/* �|�[�g */
	ulong AdrsInt;					/* �A�h���X */
	char UserID[USER_NAME_LEN+1];	/* ���[�UID */
} SOCKS4CMD;


/* �ԐM�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� */
	char Result;			/* ���U���g�R�[�h (SOCKS4_RES_xxx) */
	ushort Port;			/* �|�[�g */
	ulong AdrsInt;			/* �A�h���X */
} SOCKS4REPLY;

#define SOCKS4REPLY_SIZE	8


/*===== SOCKS5 =====*/

/* Method request�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� (SOCKS5_VER) */
	char Num;				/* ���\�b�h�̐� */
	uchar Methods[1];		/* ���\�b�h */
} SOCKS5METHODREQUEST;

#define SOCKS5METHODREQUEST_SIZE	3


/* Method reply�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� (SOCKS5_VER) */
	uchar Method;			/* ���\�b�h */
} SOCKS5METHODREPLY;

#define SOCKS5METHODREPLY_SIZE	2


/* Request�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� (SOCKS5_VER) */
	char Cmd;				/* �R�}���h (SOCKS5_CMD_xxx) */
	char Rsv;				/* �i�\��j */
	char Type;				/* �A�h���X�̃^�C�v */
							/* �Ȍ�i�ϒ������j */
	char _dummy[255+1+2];	/* �A�h���X�A�|�[�g */
} SOCKS5REQUEST;

#define SOCKS5REQUEST_SIZE 4	/* �ŏ��̌Œ蕔���̃T�C�Y */


/* Reply�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� */
	char Result;			/* ���U���g�R�[�h (SOCKS4_RES_xxx) */
	char Rsv;				/* �i�\��j */
	char Type;				/* �A�h���X�̃^�C�v */
							/* �Ȍ�i�ϒ������j */
	ulong AdrsInt;			/* �A�h���X */
	ushort Port;			/* �|�[�g */
	char _dummy[2];			/* dummy */
} SOCKS5REPLY;

#define SOCKS5REPLY_SIZE 4	/* �ŏ��̌Œ蕔���̃T�C�Y */


/* Username/Password�F��status�p�P�b�g */
typedef struct {
	char Ver;				/* �o�[�W���� */
	uchar Status;			/* �X�e�[�^�X */
} SOCKS5USERPASSSTATUS;

#define SOCKS5USERPASSSTATUS_SIZE	2



/*===== �_�C�A���O�{�b�N�X�ύX�����p =====*/

typedef struct {
	int HorMoveList[10];	/* �����ɓ��������i�̃��X�g */
	int VarMoveList[10];	/* �����ɓ��������i�̃��X�g */
	int ResizeList[10];		/* �T�C�Y�ύX���镔�i�̃��X�g */
	SIZE MinSize;			/* �ŏ��T�C�Y */
	SIZE CurSize;			/* ���݂̃T�C�Y */
} DIALOGSIZE;


/*===== ���l�ϊ��p =====*/

typedef struct {
	int Num1;
	int Num2;
} INTCONVTBL;


/*=================================================
*		�v���g�^�C�v
*=================================================*/

/*===== main.c =====*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow);
void DispWindowTitle(void);
HWND GetMainHwnd(void);
HWND GetFocusHwnd(void);
void SetFocusHwnd(HWND hWnd);
HINSTANCE GetFtpInst(void);
void DoubleClickProc(int Win, int Mode, int App);
void ExecViewer(char *Fname, int App);
void ExecViewer2(char *Fname1, char *Fname2, int App);
void AddTempFileList(char *Fname);
void SoundPlay(int Num);
char *AskHelpFilePath(void);
char *AskTmpFilePath(void);
char *AskIniFilePath(void);
int AskForceIni(void);
int BackgrndMessageProc(void);
void ResetAutoExitFlg(void);
int AskAutoExit(void);

/*===== filelist.c =====*/

int MakeListWin(HWND hWnd, HINSTANCE hInst);
void DeleteListWin(void);
HWND GetLocalHwnd(void);
HWND GetRemoteHwnd(void);
void GetListTabWidth(void);
void SetListViewType(void);
void GetRemoteDirForWnd(int Mode, int *CancelCheckWork);
void GetLocalDirForWnd(void);
void ReSortDispList(int Win, int *CancelCheckWork);
void SelectFileInList(HWND hWnd, int Type);
void FindFileInList(HWND hWnd, int Type);
// void WildCard2RegExp(char *Str);
int GetCurrentItem(int Win);
int GetItemCount(int Win);
int GetSelectedCount(int Win);
int GetFirstSelected(int Win, int All);
int GetNextSelected(int Win, int Pos, int All);
int FindNameNode(int Win, char *Name);
void GetNodeName(int Win, int Pos, char *Buf, int Max);
int GetNodeTime(int Win, int Pos, FILETIME *Buf);
int GetNodeSize(int Win, int Pos, LONGLONG *Buf);
int GetNodeAttr(int Win, int Pos, int *Buf);
int GetNodeType(int Win, int Pos);
void GetNodeOwner(int Win, int Pos, char *Buf, int Max);
void EraseRemoteDirForWnd(void);
double GetSelectedTotalSize(int Win);
void MakeSelectedFileList(int Win, int Expand, int All, FILELIST **Base, int *CancelCheckWork);
void MakeDroppedFileList(WPARAM wParam, char *Cur, FILELIST **Base);
void MakeDroppedDir(WPARAM wParam, char *Cur);
void AddRemoteTreeToFileList(int Num, char *Path, int IncDir, FILELIST **Base);
void DeleteFileList(FILELIST **Base);
FILELIST *SearchFileList(char *Fname, FILELIST *Base, int Caps);
int Assume1900or2000(int Year);
void SetFilter(int *CancelCheckWork);
void doDeleteRemoteFile(void);


/*===== toolmenu.c =====*/

int MakeToolBarWindow(HWND hWnd, HINSTANCE hInst);
void DeleteToolBarWindow(void);
HWND GetMainTbarWnd(void);
HWND GetLocalHistHwnd(void);
HWND GetRemoteHistHwnd(void);
HWND GetLocalHistEditHwnd(void);
HWND GetRemoteHistEditHwnd(void);
HWND GetLocalTbarWnd(void);
HWND GetRemoteTbarWnd(void);
int GetHideUI(void);
void MakeButtonsFocus(void);
void DisableUserOpe(void);
void EnableUserOpe(void);
int AskUserOpeDisabled(void);
void SetTransferTypeImm(int Mode);
void SetTransferType(int Type);
void DispTransferType(void);
int AskTransferType(void);
int AskTransferTypeAssoc(char *Fname, int Type);
void SaveTransferType(void);
void SetHostKanjiCodeImm(int Mode);
void SetHostKanjiCode(int Type);
void DispHostKanjiCode(void);
int AskHostKanjiCode(void);
void HideHostKanjiButton(void);
void SetHostKanaCnvImm(int Mode);
void SetHostKanaCnv(void);
void DispHostKanaCnv(void);
int AskHostKanaCnv(void);
void SetSortTypeImm(int LFsort, int LDsort, int RFsort, int RDsort);
void SetSortTypeByColumn(int Win, int Tab);
int AskSortType(int Name);
void SetSaveSortToHost(int Sw);
int AskSaveSortToHost(void);
void DispListType(void);
void SetSyncMoveMode(int Mode);
void ToggleSyncMoveMode(void);
void DispSyncMoveMode(void);
int AskSyncMoveMode(void);
void SetRemoteDirHist(char *Path);
void SetLocalDirHist(char *Path);
void AskLocalCurDir(char *Buf, int Max);
void AskRemoteCurDir(char *Buf, int Max);
void SetCurrentDirAsDirHist(void);
void DispDotFileMode(void);
void LocalRbuttonMenu(int Pos);
void RemoteRbuttonMenu(int Pos);

/*===== statuswin.c =====*/

int MakeStatusBarWindow(HWND hWnd, HINSTANCE hInst);
void DeleteStatusBarWindow(void);
HWND GetSbarWnd(void);
void DispCurrentWindow(int Win);
void DispSelectedSpace(void);
void DispLocalFreeSpace(char *Path);
void DispTransferFiles(void);
void DispDownloadSize(LONGLONG Size);

/*===== taskwin.c =====*/

int MakeTaskWindow(HWND hWnd, HINSTANCE hInst);
void DeleteTaskWindow(void);
HWND GetTaskWnd(void);
void SetTaskMsg(char *szFormat, ...);
int SaveTaskMsg(char *Fname);
void DispTaskMsg(void);
void DoPrintf(char *szFormat, ...);
void DoPrintf2(char *szFormat, ...);

/*===== hostman.c =====*/

int SelectHost(int Type);
int AddHostToList(HOSTDATA *Set, int Pos, int Level);
int CopyHostFromList(int Num, HOSTDATA *Set);
int CopyHostFromListInConnect(int Num, HOSTDATA *Set);
int SetHostBookMark(int Num, char *Bmask, int Len);
char *AskHostBookMark(int Num);
int SetHostDir(int Num, char *LocDir, char *HostDir);
int SetHostPassword(int Num, char *Pass);
int SetHostSort(int Num, int LFSort, int LDSort, int RFSort, int RDSort);
void DecomposeSortType(ulong Sort, int *LFSort, int *LDSort, int *RFSort, int *RDSort);
int AskCurrentHost(void);
void SetCurrentHost(int Num);
void CopyDefaultHost(HOSTDATA *Set);
int SearchHostName(char *Name);
void ImportFromWSFTP(void);

/*===== connect.c =====*/

void ConnectProc(int Type, int Num);
void QuickConnectProc(void);
void DirectConnectProc(char *unc, int Kanji, int Kana, int Fkanji, int TrMode);
void HistoryConnectProc(int MenuCmd);
char *AskHostAdrs(void);
int AskHostPort(void);
int AskHostNameKanji(void);
int AskHostNameKana(void);
int AskListCmdMode(void);
int AskUseNLST_R(void);
char *AskHostChmodCmd(void);
int AskHostTimeZone(void);
int AskPasvMode(void);
char *AskHostLsName(void);
int AskHostType(void);
int AskHostFireWall(void);
int AskNoFullPathMode(void);
char *AskHostUserName(void);
void SaveCurrentSetToHost(void);
int ReConnectCmdSkt(void);
// int ReConnectTrnSkt(void);
SOCKET AskCmdCtrlSkt(void);
SOCKET AskTrnCtrlSkt(void);
void SktShareProh(void);
int AskShareProh(void);
void DisconnectProc(void);
void DisconnectSet(void);
int AskConnecting(void);
SOCKET connectsock(char *host, int port, char *PreMsg, int *CancelCheckWork);
SOCKET GetFTPListenSocket(SOCKET ctrl_skt, int *CancelCheckWork);
int AskTryingConnect(void);
int SocksGet2ndBindReply(SOCKET Socket, SOCKET *Data);

/*===== cache.c =====*/

int MakeCacheBuf(int Num);
void DeleteCacheBuf(void);
int AskCached(char *Path);
int AskFreeCache(void);
void SetCache(int Num, char *Path);
void ClearCache(int Num);
int AskCurrentFileListNum(void);
void SetCurrentFileListNum(int Num);
void SaveCache(void);
void LoadCache(void);
void DeleteCache(void);
void MakeCacheFileName(int Num, char *Buf);
void CountPrevFfftpWindows(void);

/*===== ftpproc.c =====*/

void DownLoadProc(int ChName, int ForceFile, int All);
void DirectDownLoadProc(char *Fname);
void InputDownLoadProc(void);
void MirrorDownloadProc(int Notify);
void UpLoadListProc(int ChName, int All);
void UpLoadDragProc(WPARAM wParam);
void MirrorUploadProc(int Notify);
void DeleteProc(void);
void RenameProc(void);
void MoveRemoteFileProc(int);
void MkdirProc(void);
void ChangeDirComboProc(HWND hWnd);
void ChangeDirBmarkProc(int MarkID);
void ChangeDirDirectProc(int Win);
void ChangeDirDropFileProc(WPARAM wParam);
void ChmodProc(void);
BOOL CALLBACK ChmodDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void SomeCmdProc(void);
void CalcFileSizeProc(void);
void DispCWDerror(HWND hWnd);
void CopyURLtoClipBoard(void);
int ProcForNonFullpath(char *Path, char *CurDir, HWND hWnd, int Type);
void ReformToVMSstyleDirName(char *Path);
void ReformToVMSstylePathName(char *Path);
#if defined(HAVE_OPENVMS)
void ReformVMSDirName(char *DirName, int Flg);
#endif

/*===== local.c =====*/

int DoLocalCWD(char *Path);
void DoLocalMKD(char *Path);
void DoLocalPWD(char *Buf);
void DoLocalRMD(char *Path);
void DoLocalDELE(char *Path);
void DoLocalRENAME(char *Src, char *Dst);
void DispFileProperty(char *Fname);
HANDLE FindFirstFileAttr(char *Fname, WIN32_FIND_DATA *FindData, int IgnHide);
BOOL FindNextFileAttr(HANDLE hFind, WIN32_FIND_DATA *FindData, int IgnHide);

/*===== remote.c =====*/

int DoCWD(char *Path, int Disp, int ForceGet, int ErrorBell);
int DoCWDStepByStep(char *Path, char *Cur);
int DoMKD(char *Path);
void InitPWDcommand();
int DoRMD(char *Path);
int DoDELE(char *Path);
int DoRENAME(char *Src, char *Dst);
int DoCHMOD(char *Path, char *Mode);
int DoSIZE(char *Path, LONGLONG *Size);
int DoMDTM(char *Path, FILETIME *Time);
int DoQUOTE(char *CmdStr);
SOCKET DoClose(SOCKET Sock);
int DoQUIT(SOCKET ctrl_skt);
int DoDirListCmdSkt(char *AddOpt, char *Path, int Num, int *CancelCheckWork);
int CommandProcCmd(char *Reply, char *fmt, ...);
int CommandProcTrn(char *Reply, char *fmt, ...);
int command(SOCKET cSkt, char *Reply, int *CancelCheckWork, char *fmt, ...);
int SendData(SOCKET Skt, char *Data, int Size, int Mode, int *CancelCheckWork);
int ReadReplyMessage(SOCKET cSkt, char *Buf, int Max, int *CancelCheckWork, char *Tmp);
int ReadNchar(SOCKET cSkt, char *Buf, int Size, int *CancelCheckWork);
char *ReturnWSError(UINT Error);
void ReportWSError(char *Msg, UINT Error);
int ChangeFnameRemote2Local(char *Fname, int Max);
int ChangeFnameLocal2Remote(char *Fname, int Max);

/*===== getput.c =====*/

int MakeTransferThread(void);
void CloseTransferThread(void);
int AddTmpTransFileList(TRANSPACKET *Pkt, TRANSPACKET **Base);
void EraseTmpTransFileList(TRANSPACKET **Base);
int RemoveTmpTransFileListItem(TRANSPACKET **Base, int Num);

void AddTransFileList(TRANSPACKET *Pkt);
void AppendTransFileList(TRANSPACKET *Pkt);
void KeepTransferDialog(int Sw);
int AskTransferNow(void);
int AskTransferFileNum(void);
void GoForwardTransWindow(void);
void InitTransCurDir(void);
int DoDownLoad(SOCKET cSkt, TRANSPACKET *Pkt, int DirList, int *CancelCheckWork);
int CheckPathViolation(TRANSPACKET *packet);

/*===== codecnv.c =====*/

void InitTermCodeConvInfo(TERMCODECONVINFO *cInfo);
int FlushRestTermCodeConvData(TERMCODECONVINFO *cInfo);
int ConvTermCodeToCRLF(TERMCODECONVINFO *cInfo);

void InitCodeConvInfo(CODECONVINFO *cInfo);
int FlushRestData(CODECONVINFO *cInfo);
int ConvEUCtoSJIS(CODECONVINFO *cInfo);
int ConvJIStoSJIS(CODECONVINFO *cInfo);
int ConvSMBtoSJIS(CODECONVINFO *cInfo);
int ConvUTF8NtoSJIS(CODECONVINFO *cInfo); // UTF-8N�Ή�
int ConvSJIStoEUC(CODECONVINFO *cInfo);
int ConvSJIStoJIS(CODECONVINFO *cInfo);
int ConvSJIStoSMB_HEX(CODECONVINFO *cInfo);
int ConvSJIStoSMB_CAP(CODECONVINFO *cInfo);
int ConvSJIStoUTF8N(CODECONVINFO *cInfo); // UTF-8N�Ή�
void ConvAutoToSJIS(char *Text, int Pref);
int CheckKanjiCode(char *Text, int Size, int Pref);

/*===== option.c =====*/

void SetOption(int Start);
int SortSetting(void);

/*===== bookmark.c =====*/

void ClearBookMark(void);
void AddCurDirToBookMark(int Win);
int AskBookMarkText(int MarkID, char *Local, char *Remote, int Max);
void SaveBookMark(void);
void LoadBookMark(void);
int EditBookMark(void);

/*===== regexp.c =====*/

int LoadJre(void);
void ReleaseJre(void);
int AskRasUsable(void);
int AskJreUsable(void);
int GetJreVersion(void);
int JreCompileStr(char *Str);
char *JreGetStrMatchInfo(char *Str, UINT nStart);

/*===== wildcard.c =====*/

int CheckFname(char *str, char *regexp);

/*===== registory.c =====*/

void SaveRegistory(void);
int LoadRegistory(void);
void ClearRegistory(void);
void SetMasterPassword( const char* );
int GetMasterPasswordStatus(void);
int ValidateMasterPassword(void);
DWORD LoadHideDriveListRegistory(void);
void SaveSettingsToFile(void);
int LoadSettingsFromFile(void);

/*===== lvtips.c =====*/

int InitListViewTips(HWND hWnd, HINSTANCE hInst);
void DeleteListViewTips(void);
void EraseListViewTips(void);
HWND GetListViewTipsHwnd(void);
void CheckTipsDisplay(HWND hWnd, LPARAM lParam);

/*===== ras.c =====*/

void LoadRasLib(void);
void ReleaseRasLib(void);
void DisconnectRas(int Notify);
int SetRasEntryToComboBox(HWND hDlg, int Item, char *CurName);
int ConnectRas(int Dialup, int UseThis, int Notify, char *Name);

/*===== misc.c =====*/

int InputDialogBox(int Res, HWND hWnd, char *Title, char *Buf, int Max, int *Flg, int Help);
BOOL CALLBACK ExeEscDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ExeEscTextDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void SetYenTail(char *Str);
void RemoveYenTail(char *Str);
void SetSlashTail(char *Str);
void RemoveReturnCode(char *Str);
void ReplaceAll(char *Str, char Src, char Dst);
int IsDigitSym(int Ch, int Sym);
int StrAllSameChar(char *Str, char Ch);
void RemoveTailingSpaces(char *Str);
char *stristr(char *s1, char *s2);
char *GetNextField(char *Str);
int GetOneField(char *Str, char *Buf, int Max);
void RemoveComma(char *Str);
char *GetFileName(char *Path);
char *GetFileExt(char *Path);
char *GetToolName(char *Path);
void RemoveFileName(char *Path, char *Buf);
void GetUpperDir(char *Path);
void GetUpperDirEraseTopSlash(char *Path);
int AskDirLevel(char *Path);
void MakeSizeString(double Size, char *Buf);
void DispStaticText(HWND hWnd, char *Str);
int StrMultiLen(char *Str);
void RectClientToScreen(HWND hWnd, RECT *Rect);
int hex2bin(char Ch);
int SplitUNCpath(char *unc, char *Host, char *Path, char *File, char *User, char *Pass, int *Port);
int TimeString2FileTime(char *Time, FILETIME *Buf);
void FileTime2TimeString(FILETIME *Time, char *Buf, int Mode, int InfoExist);
void SpecificLocalFileTime2FileTime(FILETIME *Time, int TimeZone);
int AttrString2Value(char *Str);
void AttrValue2String(int Attr, char *Buf);
void FormatIniString(char *Str);
int SelectFile(HWND hWnd, char *Fname, char *Title, char *Filters, char *Ext, int Flags, int Save);
int SelectDir(HWND hWnd, char *Buf, int MaxLen);
void SetRadioButtonByValue(HWND hDlg, int Value, const RADIOBUTTON *Buttons, int Num);
int AskRadioButtonValue(HWND hDlg, const RADIOBUTTON *Buttons, int Num);
int xtoi(char *Str);
int CheckFileReadable(char *Fname);
int max1(int n, int m);
int min1(int n, int m);
void ExcEndianDWORD(DWORD *x);
void SwapInt(int *Num1, int *Num2);
int IsFolderExist(char *Path);
int ConvertNum(int x, int Dir, const INTCONVTBL *Tbl, int Num);
int MoveFileToTrashCan(char *Path);
LONGLONG MakeLongLong(DWORD High, DWORD Low);
char *MakeNumString(LONGLONG Num, char *Buf, BOOL Comma);

/*===== dlgsize.c =====*/

void DlgSizeInit(HWND hDlg, DIALOGSIZE *Dt, SIZE *Size);
void AskDlgSize(HWND hDlg, DIALOGSIZE *Dt, SIZE *Size);
void DlgSizeChange(HWND hDlg, DIALOGSIZE *Dt, RECT *New, int Flg);

/*===== opie.c =====*/

int Make6WordPass(int seq, char *seed, char *pass, int type, char *buf);

/*===== tool.c =====*/

void OtpCalcTool(void);

/*===== history.c =====*/

void AddHostToHistory(HOSTDATA *Host, int TrMode);
void AddHistoryToHistory(HISTORYDATA *Hist);
int AskHistoryNum(void);
void CheckHistoryNum(int Space);
void CopyHistoryToHost(HISTORYDATA *Hist, HOSTDATA *Host);
void CopyDefaultHistory(HISTORYDATA *Set);
void SetAllHistoryToMenu(void);
int GetHistoryByCmd(int MenuCmd, HISTORYDATA *Buf);
int GetHistoryByNum(int Num, HISTORYDATA *Buf);

/*===== clipboard.c =====*/

int CopyStrToClipBoard(char *Str);

/*===== diskfree.c =====*/

void LoadKernelLib(void);
void ReleaseKernelLib(void);
char *AskLocalFreeSpace(char *Path);

/*===== socket.c =====*/

int MakeSocketWin(HWND hWnd, HINSTANCE hInst);
void DeleteSocketWin(void);
struct hostent *do_gethostbyname(const char *Name, char *Buf, int Len, int *CancelCheckWork);
SOCKET do_socket(int af, int type, int protocol);
int do_connect(SOCKET s, const struct sockaddr *name, int namelen, int *CancelCheckWork);
int do_closesocket(SOCKET s);
int do_listen(SOCKET s,	int backlog);
SOCKET do_accept(SOCKET s, struct sockaddr *addr, int *addrlen);
int do_recv(SOCKET s, char *buf, int len, int flags, int *TimeOut, int *CancelCheckWork);
int do_send(SOCKET s, const char *buf, int len, int flags, int *TimeOutErr, int *CancelCheckWork);
int CheckClosedAndReconnect(void);
void CheckAllEventClosed(void);

/*===== updatebell.c =====*/

void SaveUpdateBellInfo(void);


#endif   // __COMMON_H__
