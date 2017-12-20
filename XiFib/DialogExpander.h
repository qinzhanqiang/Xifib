#ifndef _INC_EXPANDDLG_H_
#define _INC_EXPANDDLG_H_

typedef enum id_group_e
{
	ID_GROUP_GAMMA=0,
	ID_GROUP_AEAG,
	ID_GROUP_CCM,
	ID_GROUP_TRIGGER,
	ID_GROUP_TEMPER,
	ID_GROUP_ROI,
	ID_GROUP_HDR
} id_group_e;

enum oper_e
{
	OPER_READ,
	OPER_SET,
};

class CExpandDialog
{
public:
	CExpandDialog::CExpandDialog(CWnd *pDialog);
	void ToggleGroup(id_group_e id);
	void SetGroup(id_group_e id,bool show_group);
	void DisableGroup(id_group_e id_group,bool disable);
	bool IsExpandFinished();

private:
	void DoGroupOperation(oper_e operation,id_group_e id_group,bool show_group);
	CWnd* m_hDialog;
	int m_original_dialog_height;
	int m_progress;
	bool m_finished;
} ;


#endif /* _INC_EXPANDDLG_H_ */
