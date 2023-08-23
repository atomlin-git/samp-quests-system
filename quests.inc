#define MAX_PLAYERS_QUESTS                  3
#define MAX_PLAYER_SIMULTANEOUSLY_QUESTS    15

#define QUEST_TYPE_TASK         0
#define QUEST_TYPE_SPECIAL      1
#define QUEST_TYPE_ELEMENTARY   2

#define QUEST_DIALOG_ACCEPT     	30000
#define QUEST_DIALOG_LIST       	30001
#define QUEST_DIALOG_INFORMATION 	30002

new QUEST_TYPES[3][] = {"{FF6347}[Задание]{FFFFFF}", "{42B02C}[Особый]{FFFFFF}", "{FFFF00}[Начальный]{FFFFFF}"};

enum QUEST_ENUM
{
	QUEST_UNIC_ID,
	QUEST_ID,
	QUEST_PROGRESS,
	QUEST_TYPE
};
new QUEST_INFO[MAX_PLAYERS][MAX_PLAYERS_QUESTS][QUEST_ENUM];

enum QUEST_INFO_ENUM
{
	QUEST_NAME[32],
	QUEST_INF[600],
	QUEST_REQUERIED,
	QUEST_TYPE
};
new QUEST_INFORMATION[MAX_PLAYERS_QUESTS][QUEST_INFO_ENUM] = 
{
	{"Пора работать!", "{FFFFFF}Вижу у тебя нет денег, но это не беда! В нашем штате есть предприятие \"Шахта\" \nТам можно заработать отличные деньги. Направляйся туда и перенеси {cccccc}200КГ{FFFFFF} руды.\n\n\nНаграда за выполнение - {cccccc}1 EXP{FFFFFF}, {cccccc}500${FFFFFF}.", 1, QUEST_TYPE_TASK},
	{"Получаем права!", "{FFFFFF}Так, ты поработал на шахте и теперь у тебя есть деньги на права!\nДля выполнения задания тебе нужно сдать на права в нашей автошколе\nНайти ты её можешь в своём GPS-навигаторе {CCCCCC}(( /gps -> Важные места -> Автошкола )){FFFFFF}\n\nНаграда за выполнение - {cccccc}2 EXP{FFFFFF}, {cccccc}250${FFFFFF}", 1, QUEST_TYPE_TASK},
	{"Ох.. Снова работа..", "{FFFFFF}Теперь, когда у тебя есть деньги и права на вождение авто, ты можешь пойти работать на ферму!\nДля выполнения задания вам нужно получить место главного фермера и выполнить 5 перевозок\n\nНаграда за выполнение - {cccccc}3 EXP{FFFFFF}, {cccccc}250${FFFFFF}.", 5, QUEST_TYPE_TASK}
};

new PLAYER_QUESTS_SELECT[MAX_PLAYERS][MAX_PLAYER_SIMULTANEOUSLY_QUESTS];
new PLAYER_QUESTS_LOADINGS_COUNT[MAX_PLAYERS];
new PLAYER_QUESTS_LOADINGS[MAX_PLAYERS][MAX_PLAYERS_QUESTS];
new PLAYER_QUESTS_RANGE[MAX_PLAYERS][2];
new PLAYER_DIALOG_SELECT_QUEST[MAX_PLAYERS];
new PLAYER_DIALOG_SELECTED_QUEST[MAX_PLAYERS];

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case QUEST_DIALOG_ACCEPT:
		{
			if(!response) return SHOW_QUESTS_DIALOG(playerid, PLAYER_QUESTS_RANGE[playerid][0], PLAYER_QUESTS_RANGE[playerid][1]), PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;

			new TEMPORARY_QUESTS_COUNT;

			for(new i; i < PLAYER_QUESTS_LOADINGS_COUNT[playerid]; i++)
			{
				if(QUEST_INFO[playerid][i][QUEST_PROGRESS] < 1000 && QUEST_INFO[playerid][i][QUEST_PROGRESS] != -1) TEMPORARY_QUESTS_COUNT++;
			}

			if(TEMPORARY_QUESTS_COUNT >= MAX_PLAYER_SIMULTANEOUSLY_QUESTS) return SCM(playerid, COLOR_GREY, "Одновременно можно принимать не более 15 квестов!");
			if(PLAYER_DIALOG_SELECT_QUEST[playerid] == -1) return SCM(playerid, COLOR_GREY, "Произошла ошибка (#2) откройте диалог выбора квестов снова!");


			if(PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]] != -1)
			{
				if(QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_PROGRESS] == 1000) return SCM(playerid, COLOR_GREY, "Вы уже выполняли данное задание!");
				if(QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_PROGRESS] != -1 && (QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_PROGRESS] < QUEST_INFORMATION[PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_REQUERIED])) return SCM(playerid, COLOR_GREY, "Ты ещё не выполнил всех условий! Подсказки доступны в /quest");
			}

			switch(PLAYER_DIALOG_SELECT_QUEST[playerid])
			{
				case 1:
				{
					if(PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]] != -1)
					{
						if(QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_PROGRESS] == 999)
						{
							SCM(playerid, COLOR_GREY, "Ты молодец, справился! Держи за это 200$");
							GiveMoney(playerid, 200);
							SAVE_QUEST(playerid, 1000, PLAYER_DIALOG_SELECT_QUEST[playerid], QUEST_INFORMATION[PLAYER_QUESTS_LOADINGS[playerid][PLAYER_DIALOG_SELECT_QUEST[playerid]]][QUEST_TYPE]);
							return 1;
						}
					}
					SCM(playerid, COLOR_GREY, "Ну.. За работу! Я поставил тебе метку в GPS-навигаторе, принеси мне это, и считай - справился");
					EnableGPSForPlayer(playerid, 1240.0363, -1710.1296, 13.3828);
					SAVE_QUEST(playerid, 0, PLAYER_DIALOG_SELECT_QUEST[playerid], QUEST_INFORMATION[PLAYER_DIALOG_SELECT_QUEST[playerid]][QUEST_TYPE]);
					PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
				}
				default: SCM(playerid, COLOR_GREY, "Произошла ошибка! Информация по квесту отсутствует (#1)");
			}

			PLAYER_QUESTS_RANGE[playerid][0] = -1;
			PLAYER_QUESTS_RANGE[playerid][1] = -1;

			PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
		}
		case QUEST_DIALOG_LIST:
		{
			if(!response)
			{
				PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
				PLAYER_QUESTS_RANGE[playerid][0] = -1;
				PLAYER_QUESTS_RANGE[playerid][1] = -1;
				return 1;
			}

			if((PLAYER_QUESTS_RANGE[playerid][0] < 0 || PLAYER_QUESTS_RANGE[playerid][1] > MAX_PLAYERS_QUESTS) || PLAYER_QUESTS_RANGE[playerid][1] < PLAYER_QUESTS_RANGE[playerid][0]) return SCM(playerid, COLOR_GREY, "Произошла ошибка! Откройте диалог квестов снова");
			 
			PLAYER_DIALOG_SELECT_QUEST[playerid] = listitem + PLAYER_QUESTS_RANGE[playerid][0] + 1;
			SPD(playerid, QUEST_DIALOG_ACCEPT, DIALOG_STYLE_MSGBOX, "{FFFFFF}Задания", QUEST_INFORMATION[PLAYER_DIALOG_SELECT_QUEST[playerid]][QUEST_INF], "Принять", "Закрыть");
		}
		case QUEST_DIALOG_INFORMATION:
		{
			if(response)
			{
				if(PLAYER_DIALOG_SELECT_QUEST[playerid] == 1)
				{
					switch(listitem)
					{
						case 0:
						{
							new string_quest_TITLE[32], string[500];
							format(string_quest_TITLE, sizeof(string_quest_TITLE), "%s", QUEST_INFORMATION[PLAYER_DIALOG_SELECTED_QUEST[playerid]][QUEST_NAME]);
							format(string, sizeof(string), "%s", QUEST_INFORMATION[PLAYER_DIALOG_SELECTED_QUEST[playerid]][QUEST_INF]);
							SPD(playerid, QUEST_DIALOG_INFORMATION, DIALOG_STYLE_MSGBOX, string_quest_TITLE, string, "Принять", "Назад");
							PLAYER_DIALOG_SELECT_QUEST[playerid] = 2;
						}
						case 1:
						{
							if(QUEST_INFO[playerid][PLAYER_DIALOG_SELECTED_QUEST[playerid]][QUEST_TYPE] == 1) return SCM(playerid, COLOR_GREEN, "Нельзя отказываться от выполнения квеста с типом \"Особый\"");

							new qid = PLAYER_DIALOG_SELECTED_QUEST[playerid];

							static const FMT_QUERY_QUEST[] = "SELECT * FROM `quests` WHERE `QID` = '%d' AND `OWNER` = '%d' AND PROGRESS < 1000";
							new QUERY_QUEST[sizeof(FMT_QUERY_QUEST)+444];
							mysql_format(dbHandle, QUERY_QUEST, sizeof(QUERY_QUEST), FMT_QUERY_QUEST, qid+1, player_info[playerid][ID]);
							new Cache:RESULT_QUERY = mysql_query(dbHandle, QUERY_QUEST);

							new RESULTED_QUEST_INFO = cache_num_rows();

							if(!RESULTED_QUEST_INFO) return SCM(playerid, COLOR_GREY, "Ошибка в попытке отказаться от квеста #1");
							
							cache_delete(RESULT_QUERY)

							static const FMT_QUERY_QUEST2[] = "DELETE FROM `quests` WHERE `QID` = '%d' AND `OWNER` = '%d' AND PROGRESS < 1000";
							new QUERY_QUEST2[sizeof(FMT_QUERY_QUEST2)+222];
							mysql_format(dbHandle, QUERY_QUEST2, sizeof(QUERY_QUEST2), FMT_QUERY_QUEST2, qid+1, player_info[playerid][ID]);
							mysql_query(dbHandle, QUERY_QUEST2);

							new string_quest_decline[100];
							format(string_quest_decline, sizeof(string_quest_decline), "Вы отказались от квестового задания \"%s\"", QUEST_INFORMATION[qid][QUEST_NAME]);
							SCM(playerid, COLOR_GREEN, string_quest_decline);

							PLAYER_QUESTS_LOADINGS[playerid][QUEST_INFO[playerid][qid][QUEST_ID]] = -1;

							QUEST_INFO[playerid][qid][QUEST_ID] = -1;
							QUEST_INFO[playerid][qid][QUEST_PROGRESS] = 1001;
							QUEST_INFO[playerid][qid][QUEST_UNIC_ID] = -1;
							QUEST_INFO[playerid][qid][QUEST_TYPE] = -1;

							PLAYER_QUESTS_LOADINGS_COUNT[playerid]--;

							PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
							return 1;
						}
					}
					PLAYER_DIALOG_SELECTED_QUEST[playerid] = PLAYER_QUESTS_SELECT[playerid][listitem];
					return 1;
				}
				PLAYER_DIALOG_SELECT_QUEST[playerid] = 1;
				SPD(playerid, QUEST_DIALOG_INFORMATION, DIALOG_STYLE_LIST, "{cccccc}Выбор действия с квестовым заданием", "{FFFFFF}Подсказка квестового задания\n{BE2D2D}Отказаться от квестового задания", "Принять", "Назад");
			}
			if(!response)
			{
				if(PLAYER_DIALOG_SELECT_QUEST[playerid] == 1)
				{
					callcmd::quest(playerid);
					PLAYER_DIALOG_SELECT_QUEST[playerid] = 0;
				}
				if(PLAYER_DIALOG_SELECT_QUEST[playerid] == 2)
				{
					PLAYER_DIALOG_SELECT_QUEST[playerid] = 1;
					SPD(playerid, QUEST_DIALOG_INFORMATION, DIALOG_STYLE_LIST, "{cccccc}Выбор действия с квестовым заданием", "{FFFFFF}Подсказка квестового задания\n{BE2D2D}Отказаться от квестового задания", "Принять", "Назад");
				}
			}
		}
	}
	return _OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}

public OnPlayerDisconnect(playerid, reason)
{
	if(PLAYER_QUESTS_LOADINGS_COUNT[playerid])
	{
		for(new i; i < PLAYER_QUESTS_LOADINGS_COUNT[playerid]; i++)
		{
			if(QUEST_INFO[playerid][i][QUEST_PROGRESS] < 999 && QUEST_INFO[playerid][i][QUEST_ID] != -1)
			{
				SAVE_QUEST(playerid, QUEST_INFO[playerid][i][QUEST_PROGRESS], QUEST_INFO[playerid][i][QUEST_ID], QUEST_INFO[playerid][i][QUEST_TYPE]);
			}
		}
	}

	return __OnPlayerDisconnect(playerid, reason);
}

public OnPlayerConnect(playerid)
{
	PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
	PLAYER_QUESTS_LOADINGS_COUNT[playerid] = -1;

	PLAYER_QUESTS_RANGE[playerid][0] = -1;
	PLAYER_QUESTS_RANGE[playerid][1] = -1;

	for(new i; i < MAX_PLAYER_SIMULTANEOUSLY_QUESTS; i++) PLAYER_QUESTS_SELECT[playerid][i] = -1;
	
	return __OnPlayerConnect(playerid);
}

CMD:quest(playerid)
{
	if(PLAYER_QUESTS_LOADINGS_COUNT[playerid] < 1) return SCM(playerid, COLOR_GREY, "У вас нет принятых квестов!");

	new QUESTS_SHOW_DIALOG[600];
	new STRING_QUESTS_SHOW_DIALOG[150];

	QUESTS_SHOW_DIALOG = "{cccccc}Тип\t{cccccc}Название\t{cccccc}Прогресс\t{cccccc}Статус\n{FFFFFF}";

	new num;
	for(new i; i < PLAYER_QUESTS_LOADINGS_COUNT[playerid]; i++)
	{
		if(i > sizeof(QUEST_INFORMATION)) return SCM(playerid, COLOR_GREY, "Ошибка загруки информации о квесте #2");
		if(QUEST_INFO[playerid][i][QUEST_PROGRESS] < 1000)
		{
			if(QUEST_INFO[playerid][i][QUEST_ID] == -1) return SCM(playerid, COLOR_GREY, "Ошибка загруки информации о квесте #5");
			if(QUEST_INFO[playerid][i][QUEST_ID] <= 0) return SCM(playerid, COLOR_GREY, "Ошибка загруки информации о квесте #4");
			if(QUEST_INFO[playerid][i][QUEST_ID] > sizeof(QUEST_INFORMATION)) return SCM(playerid, COLOR_GREY, "Ошибка загруки информации о квесте #2.1");
			if(QUEST_INFO[playerid][i][QUEST_PROGRESS] >= 1000) return 1;

			STRING_QUESTS_SHOW_DIALOG[0] = EOS;
			format(STRING_QUESTS_SHOW_DIALOG, sizeof(STRING_QUESTS_SHOW_DIALOG), "%s\t{FFFFFF}[%s]\t[%d/%d]\t%s\n", QUEST_TYPES[QUEST_INFO[playerid][i][QUEST_TYPE]], QUEST_INFORMATION[QUEST_INFO[playerid][i][QUEST_ID]-1][QUEST_NAME], (QUEST_INFO[playerid][i][QUEST_PROGRESS] == 999) ? (QUEST_INFORMATION[i][QUEST_REQUERIED]) : (QUEST_INFO[playerid][i][QUEST_PROGRESS]), QUEST_INFORMATION[i][QUEST_REQUERIED], (QUEST_INFO[playerid][i][QUEST_PROGRESS] == 999) ? ("[Можно завершить]") : ("[В процессе]"));
			strcat(QUESTS_SHOW_DIALOG, STRING_QUESTS_SHOW_DIALOG);

			PLAYER_QUESTS_SELECT[playerid][num] = QUEST_INFO[playerid][i][QUEST_ID]-1;

			num++;
		}
	}

	PLAYER_DIALOG_SELECT_QUEST[playerid] = -1;
	if(num <= 0) return SCM(playerid, COLOR_GREY, "У вас нет принятых квестов!");
	if(num > 0) SPD(playerid, QUEST_DIALOG_INFORMATION, DIALOG_STYLE_TABLIST_HEADERS, "{cccccc}Квестовые задания", QUESTS_SHOW_DIALOG, "Выбрать", "Закрыть");

	return 1;
}

CMD:setqueststat(playerid, params[])
{
	if(!TEST_SERVER && player_info[playerid][ADMIN] < 7) return 0;
	if(sscanf(params, "dddd", params[0], params[1], params[2], params[3])) return SCM(playerid, COLOR_GREY, "Используйте /setqueststat [ид игрока] [ид квеста] [тип квеста] [прогресс квеста]");
	if(!GetPVarInt(params[0], "logged")) return SCM(playerid, COLOR_GREY, "Игрок не вступил в игру!");
	if(params[1] > sizeof(QUEST_INFORMATION)) return SCM(playerid, COLOR_GREY, "Такого квеста не существует!");
	SAVE_QUEST(params[0], params[3], params[1], params[2]);
	new string[333];
	format(string, sizeof(string), "Вы успешно изменили параметры квеста %s[%d] на [type]: %d, [prog]: %d. Для игрока %s[%d]", QUEST_INFORMATION[params[1]-1][QUEST_NAME], params[1], params[2], params[3], pName(params[0]), params[0]);
	SCM(playerid, COLOR_GREEN, string);
	string[0] = EOS;
	format(string, sizeof(string), "Администратор %s[%d] изменил параметры вашего квеста %s[%d] на [type]: %d, [prog]: %d", pName(playerid), playerid, QUEST_INFORMATION[params[1]-1][QUEST_NAME], params[1], params[2], params[3]);
	SCM(params[0], COLOR_GREEN, string);
	return 1;
}

stock GIVE_QUEST_PROGRESS(playerid, QUEST, PROG)
{

    if(!IsPlayerConnected(playerid)) return 1;
    if(QUEST > MAX_PLAYERS_QUESTS) return 1;
    if(QUEST_INFO[playerid][QUEST][QUEST_PROGRESS] == 1000 || QUEST_INFO[playerid][QUEST][QUEST_PROGRESS] == 1001) return SCM(playerid, COLOR_GREY, "Ошибка выдачи прогресса #1");
    if(QUEST_INFO[playerid][QUEST][QUEST_PROGRESS] == 999)
    {
        new FORMATTED_STRING[90];
        format(FORMATTED_STRING, sizeof(FORMATTED_STRING), "Задание \"%s\" успешно выполнено! Вы можете получить награду", QUEST_INFORMATION[QUEST_INFO[playerid][QUEST][QUEST_ID]-1][QUEST_NAME]);
        SCM(playerid, COLOR_GREEN, FORMATTED_STRING);
        return 1;
    }

    QUEST_INFO[playerid][QUEST][QUEST_PROGRESS] += PROG;

    if(QUEST_INFO[playerid][QUEST][QUEST_PROGRESS] >= QUEST_INFORMATION[QUEST_INFO[playerid][QUEST][QUEST_ID]][QUEST_REQUERIED])
    {
        SAVE_QUEST(playerid, 999, QUEST+1, QUEST_INFO[playerid][QUEST][QUEST_TYPE]);
		
		new FORMATTED_STRING[90];
        format(FORMATTED_STRING, sizeof(FORMATTED_STRING), "Задание \"%s\" успешно выполнено! Вы можете получить награду", QUEST_INFORMATION[QUEST_INFO[playerid][QUEST][QUEST_ID]-1][QUEST_NAME]);
        SCM(playerid, COLOR_GREEN, FORMATTED_STRING);
    }

    return 1;
}

stock SAVE_QUEST(playerid, PROGRESS, QID_T, TYPE = 0)
{
    if(!IsPlayerConnected(playerid)) return 1;

	static const FMT_QUERY_QUEST[] = "SELECT * FROM `quests` WHERE `QID` = '%d' AND `OWNER` = '%d'";
	new QUERY_QUEST[sizeof(FMT_QUERY_QUEST)+24];
	mysql_format(dbHandle, QUERY_QUEST, sizeof(QUERY_QUEST), FMT_QUERY_QUEST, QID_T, player_info[playerid][ID]);
	new Cache:RESULT_QUERY = mysql_query(dbHandle, QUERY_QUEST);

	new RESULTED_QUEST_INFO = cache_num_rows();

	if(QID_T <= 0) return SCM(playerid, COLOR_GREY, "Ошибка сохранения квеста #2");

	if(!RESULTED_QUEST_INFO)
	{
		if(PLAYER_QUESTS_LOADINGS[playerid][QID_T] != -1) return SCM(playerid, COLOR_GREY, "Ошибка сохранения квеста #1");

		static const fmt_query[] = "INSERT INTO `quests` (`QID`, `PROGRESS`, `OWNER`, `TYPE`) VALUES ('%d', '%d', '%d', '%d')";
		new query[sizeof(fmt_query)+53];
		mysql_format(dbHandle, query, sizeof(query), fmt_query, QID_T, PROGRESS, player_info[playerid][ID], TYPE);
		mysql_query(dbHandle, query, false);

		static const fmt_query2[] = "SELECT * FROM `quests` WHERE `OWNER` = '%d'";
    	new query2[sizeof(fmt_query2)+15];
		mysql_format(dbHandle, query2, sizeof(query2), fmt_query2, player_info[playerid][ID]);
		mysql_tquery(dbHandle, query2, "LOAD_PLAYER_QUESTS", "i", playerid);
	} else {
		QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][QID_T]][QUEST_PROGRESS] = PROGRESS;
		QUEST_INFO[playerid][PLAYER_QUESTS_LOADINGS[playerid][QID_T]][QUEST_TYPE] = TYPE;

		static const fmt_query[] = "UPDATE `quests` SET `PROGRESS` = '%d', `TYPE` = '%d' WHERE `OWNER` = '%d' AND `QID` = '%d'";
		new query[sizeof(fmt_query)+42];
		mysql_format(dbHandle, query, sizeof(query), fmt_query, PROGRESS, TYPE, player_info[playerid][ID], QID_T);
		mysql_tquery(dbHandle, query);
	}

	//SEND_DEBUG_MESSAGE(-1, COLOR_RED, "[debug] PROGRESS - %d | RESULT: %s | ID - %d | TYPE - %d", PROGRESS, (!RESULTED_QUEST_INFO) ? ("INSERT") : ("UPDATE"), QID_T, TYPE);

	cache_delete(RESULT_QUERY);

	return 1;
}

stock SHOW_QUESTS_DIALOG(playerid, OT, DO)
{
	new string_dialog_quests[200];
	new dialog_quests[1200];

	PLAYER_QUESTS_RANGE[playerid][0] = OT;
	PLAYER_QUESTS_RANGE[playerid][1] = DO;

	for(new i = OT; i < DO; i++)
	{
		new our_string[16];
		switch(QUEST_INFO[playerid][i][QUEST_PROGRESS])
		{
			case 1001: our_string = "Доступен";
			case 0..998: our_string = "В процессе";
			case 999: our_string = "Можно завершить";
			case 1000: our_string = "Выполнен";
		}
		string_dialog_quests[0] = EOS;
		format(string_dialog_quests, sizeof(string_dialog_quests), "%s %s\t{CCCCCC}[%s]{FFFFFF}\n", QUEST_TYPES[QUEST_INFORMATION[i][QUEST_TYPE]], QUEST_INFORMATION[i][QUEST_NAME], our_string);
		strcat(dialog_quests, string_dialog_quests);
	}

	SPD(playerid, QUEST_DIALOG_LIST, DIALOG_STYLE_TABLIST, "{FFFFFF}Задания", dialog_quests, "Выбрать", "Закрыть");
	return 1;
}

forward LOAD_PLAYER_QUESTS(playerid);
public LOAD_PLAYER_QUESTS(playerid)
{
	new rows;
	cache_get_row_count(rows);

	if(rows > MAX_PLAYERS_QUESTS) return SCM(playerid, COLOR_GREY, "Ошибка загрузки квестов #1");

	for(new i; i < MAX_PLAYERS_QUESTS; i++)
	{
		QUEST_INFO[playerid][i][QUEST_UNIC_ID] = -1;
		QUEST_INFO[playerid][i][QUEST_ID] = -1;
		QUEST_INFO[playerid][i][QUEST_PROGRESS] = 1001;
		QUEST_INFO[playerid][i][QUEST_TYPE] = -1;

		PLAYER_QUESTS_LOADINGS[playerid][i] = -1;
	}

	if(rows)
	{
		for(new i; i < cache_num_rows(); i++)
		{
			cache_get_value_name_int(i, "ID", QUEST_INFO[playerid][i][QUEST_UNIC_ID]);
			cache_get_value_name_int(i, "QID", QUEST_INFO[playerid][i][QUEST_ID]);
			cache_get_value_name_int(i, "PROGRESS", QUEST_INFO[playerid][i][QUEST_PROGRESS]);
			cache_get_value_name_int(i, "TYPE", QUEST_INFO[playerid][i][QUEST_TYPE]);

			PLAYER_QUESTS_LOADINGS[playerid][QUEST_INFO[playerid][i][QUEST_ID]] = i;
		}

		PLAYER_QUESTS_LOADINGS_COUNT[playerid] = rows;
	}

	return 1;
}

/////////////////////////////////////////////////////////////////

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse _OnDialogResponse
#if defined _OnDialogResponse
	forward _OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect __OnPlayerDisconnect
#if defined __OnPlayerDisconnect
	forward __OnPlayerDisconnect(playerid, reason);
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect __OnPlayerConnect
#if defined __OnPlayerConnect
	forward __OnPlayerConnect(playerid);
#endif