###### Система нормально не тестилась, размеры string переменных нормально не просчитывались, сделано для портфолио

###### Подгрузка информации о квестах игрока:

```PAWN
static const fmt_query[] = "SELECT * FROM `quests` WHERE `OWNER` = '%d'";
new query[sizeof(fmt_query)+15];
mysql_format(dbHandle, query, sizeof(query), fmt_query, player_info[playerid][ID]);
mysql_tquery(dbHandle, query, "LOAD_PLAYER_QUESTS", "d", playerid);
```


###### Видео-демонстрация: https://youtu.be/_Z2X_DIpXIc


###### TODO:

- [ ] глобальный рефакторинг;
- [ ] переписать принцип сохранения;
