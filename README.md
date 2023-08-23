Удобная и практичная система квестовых заданий

Подгружаю данные о квестах подобным образом: (После прогрузки аккаунта)

<code>
static const fmt_query[] = "SELECT * FROM `quests` WHERE `OWNER` = '%d'";
    	new query[sizeof(fmt_query)+15];
		  mysql_format(dbHandle, query, sizeof(query), fmt_query, player_info[playerid][ID]);
		  mysql_tquery(dbHandle, query, "LOAD_PLAYER_QUESTS", "d", playerid);
</code>


Видео-демонстрация: https://youtu.be/_Z2X_DIpXIc
