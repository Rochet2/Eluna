/*
* Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef QUERYMETHODS_H
#define QUERYMETHODS_H

#ifndef TRINITY
#define RESULT  result
#else
#define RESULT  (*result)
#endif

/***
 * The result of a database query.
 *
 * E.g. the return value of [Global:WorldDBQuery].
 * Note: You can traverse the rows of a query only ONCE.
 * This means you can only use [ElunaQuery:GetRows] once or you can loop through all rows with [ElunaQuery:NextRow] only once.
 * You can however use [ElunaQuery:GetColumnCount] and similar multiple times.
 *
 * Inherits all methods from: none
 */
namespace LuaQuery
{
    void CheckFields(lua_State* L, ElunaQuery* result)
    {
        if (Eluna::CHECKVAL<uint32>(L, 2) >= RESULT->GetFieldCount())
            luaL_argerror(L, 2, "invalid field index");
    }

    /**
     * Returns `true` if the specified column of the current row is `nullptr`, otherwise `false`.
     *
     * @param uint32 column
     * @return bool isNull
     */
    int IsNull(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);

#ifndef TRINITY
        Eluna::Push(L, RESULT->Fetch()[col].IsNULL());
#else
        Eluna::Push(L, RESULT->Fetch()[col].IsNull());
#endif
        return 1;
    }

    /**
     * Returns the number of columns in the result set.
     *
     * @return uint32 columnCount
     */
    int GetColumnCount(lua_State* L, ElunaQuery* result)
    {
        Eluna::Push(L, RESULT->GetFieldCount());
        return 1;
    }

    /**
     * Returns the number of rows in the result set.
     *
     * @return uint32 rowCount
     */
    int GetRowCount(lua_State* L, ElunaQuery* result)
    {
        if (RESULT->GetRowCount() > (uint32)-1)
            Eluna::Push(L, std::numeric_limits<uint32>::max());
        else
            Eluna::Push(L, static_cast<uint32>(RESULT->GetRowCount()));
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a boolean.
     *
     * @param uint32 column
     * @return bool data
     */
    int GetBool(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetBool());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to an unsigned 8-bit integer.
     *
     * @param uint32 column
     * @return uint8 data
     */
    int GetUInt8(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetUInt8());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to an unsigned 16-bit integer.
     *
     * @param uint32 column
     * @return uint16 data
     */
    int GetUInt16(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetUInt16());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to an unsigned 32-bit integer.
     *
     * @param uint32 column
     * @return uint32 data
     */
    int GetUInt32(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetUInt32());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to an unsigned 64-bit integer.
     *
     * @param uint32 column
     * @return uint64 data
     */
    int GetUInt64(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetUInt64());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a signed 8-bit integer.
     *
     * @param uint32 column
     * @return int8 data
     */
    int GetInt8(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetInt8());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a signed 16-bit integer.
     *
     * @param uint32 column
     * @return int16 data
     */
    int GetInt16(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetInt16());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a signed 32-bit integer.
     *
     * @param uint32 column
     * @return int32 data
     */
    int GetInt32(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetInt32());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a signed 64-bit integer.
     *
     * @param uint32 column
     * @return int64 data
     */
    int GetInt64(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetInt64());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a 32-bit floating point value.
     *
     * @param uint32 column
     * @return float data
     */
    int GetFloat(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetFloat());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a 64-bit floating point value.
     *
     * @param uint32 column
     * @return double data
     */
    int GetDouble(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);
        Eluna::Push(L, RESULT->Fetch()[col].GetDouble());
        return 1;
    }

    /**
     * Returns the data in the specified column of the current row, casted to a string.
     *
     * @param uint32 column
     * @return string data
     */
    int GetString(lua_State* L, ElunaQuery* result)
    {
        uint32 col = Eluna::CHECKVAL<uint32>(L, 2);
        CheckFields(L, result);

#ifndef TRINITY
        Eluna::Push(L, RESULT->Fetch()[col].GetString());
#else
        Eluna::Push(L, RESULT->Fetch()[col].GetCString());
#endif
        return 1;
    }

    /**
     * Advances the [ElunaQuery] to the next row in the result set.
     *
     * *Do not* call this immediately after a query, or you'll skip the first row.
     *
     * Returns `false` if there was no new row, otherwise `true`.
     *
     * @return bool hadNextRow
     */
    int NextRow(lua_State* L, ElunaQuery* result)
    {
        Eluna::Push(L, RESULT->NextRow());
        return 1;
    }

    void Helper_PushRowAsTable(lua_State* L, ElunaQuery* result, bool asArray)
    {
        uint32 col = RESULT->GetFieldCount();
        Field* row = RESULT->Fetch();

        if (asArray)
            lua_createtable(L, col, 0);
        else
            lua_createtable(L, 0, col);
        int tbl = lua_gettop(L);

#ifndef TRINITY
        const QueryFieldNames& names = RESULT->GetFieldNames();
#endif

        for (uint32 i = 0; i < col; ++i)
        {
            auto const & field = RESULT->GetFieldInfo(i);

#ifdef TRINITY
            if (!asArray)
                Eluna::Push(L, field.name);
            if (row[i].IsNull())
                Eluna::Push(L);
#else
            if (!asArray)
                Eluna::Push(L, names[i]);
            if (row[i].IsNULL())
                Eluna::Push(L);
#endif
            else
            {
                if (field.flags & UNSIGNED_FLAG == UNSIGNED_FLAG)
                {
                    switch (row[i].GetType())
                    {
                    case MYSQL_TYPE_TINY:
                        Eluna::Push(L, row[i].GetUInt8()); break;
                    case MYSQL_TYPE_SHORT:
                        Eluna::Push(L, row[i].GetUInt16()); break;
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_LONG:
                        Eluna::Push(L, row[i].GetUInt32()); break;
                    case MYSQL_TYPE_LONGLONG:
                        Eluna::Push(L, row[i].GetUInt64()); break;
                    case MYSQL_TYPE_FLOAT:
                        Eluna::Push(L, row[i].GetFloat()); break;
                    case MYSQL_TYPE_DOUBLE:
                        Eluna::Push(L, row[i].GetDouble()); break;
                        break;
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_BLOB:
                        // null case already checked so should be able to be cstr and std::string
                        Eluna::Push(L, row[i].GetString()); break;
                    case MYSQL_TYPE_BIT:
                        Eluna::Push(L, row[i].GetBool()); break;
                    case MYSQL_TYPE_NULL:
                        Eluna::Push(L); break;
                    default:
                        Eluna::Push(L, row[i].GetString());
                        break;
                    }
                }
                else
                {
                    switch (row[i].GetType())
                    {
                    case MYSQL_TYPE_TINY:
                        Eluna::Push(L, row[i].GetInt8()); break;
                    case MYSQL_TYPE_SHORT:
                        Eluna::Push(L, row[i].GetInt16()); break;
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_LONG:
                        Eluna::Push(L, row[i].GetInt32()); break;
                    case MYSQL_TYPE_LONGLONG:
                        Eluna::Push(L, row[i].GetInt64()); break;
                    case MYSQL_TYPE_FLOAT:
                        Eluna::Push(L, row[i].GetFloat()); break;
                    case MYSQL_TYPE_DOUBLE:
                        Eluna::Push(L, row[i].GetDouble()); break;
                        break;
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_BLOB:
                        // null case already checked so should be able to be cstr and std::string
                        Eluna::Push(L, row[i].GetString()); break;
                    case MYSQL_TYPE_BIT:
                        Eluna::Push(L, row[i].GetBool()); break;
                    case MYSQL_TYPE_NULL:
                        Eluna::Push(L); break;
                    default:
                        Eluna::Push(L, row[i].GetString());
                        break;
                    }
                }
            }

            if (asArray)
                lua_rawseti(L, tbl, i+1); // lua index starts at 1
            else
                lua_rawset(L, tbl);
        }

        lua_settop(L, tbl);
    }

    /**
    * Returns a table filled with the values from the row.
    *
    * All numerical values will be numbers and everything else is returned as a string.
    * If asArray is false, the table will have field names as keys and field value as the value.
    * If asArray is true, the table will have just field value as the value and a running number as the key.
    * Note that the array form can contain nil values and you can not treat it as a sequence.
    *
    * **For example,** the query:
    *
    *     SELECT entry, subname, name FROM creature_template
    *
    * would result in a table like:
    *
    *     { entry = 123, subname = nil, name = "some creature name" } -- asArray = false
    *     { 123, nil, "some creature name" } -- asArray = true
    *
    * To move to next row use [ElunaQuery:NextRow].
    *
    * @param bool asArray = false
    * @return table rowData : table filled with row data
    */
    int GetRow(lua_State* L, ElunaQuery* result)
    {
        bool asArray = Eluna::CHECKVAL<bool>(L, 2, false);
        Helper_PushRowAsTable(L, result, asArray);
        return 1;
    }

    /**
    * Returns a table filled with tables that have the values from the whole query.
    *
    * All numerical values will be numbers and everything else is returned as a string.
    * If asArray is false, the tables will have field names as keys and field value as the value.
    * If asArray is true, the tables will have just field value as the value and a running number as the key.
    * Note that the array form of a row can contain nil values and you can not treat it as a sequence.
    *
    * **For example,** the query:
    *
    *     SELECT entry, subname, name FROM creature_template
    *
    * would result in a table like:
    *
    *     { -- asArray = false
    *       { entry = 123, subname = nil, name = "some creature name" },
    *       { entry = 123, subname = nil, name = "some creature name" },
    *     }
    *
    *     { -- asArray = true
    *       { 123, nil, "some creature name" },
    *       { 123, nil, "some creature name" },
    *     }
    *
    * @param bool asArray = false
    * @return table rowData : table filled with tables of row data
    */
    int GetRows(lua_State* L, ElunaQuery* result)
    {
        bool asArray = Eluna::CHECKVAL<bool>(L, 2, false);

        lua_createtable(L, RESULT->GetRowCount(), asArray ? 1 : 0);
        int tbl = lua_gettop(L);

        size_t i = 1;
        do
        {
            Helper_PushRowAsTable(L, result, asArray);
            lua_rawseti(L, tbl, i++);
        } while (RESULT->NextRow());

        lua_settop(L, tbl);
        return 1;
    }

    /**
    * Returns a table filled with query column names.
    *
    * Example:
    *
    *     { "entry", "subname", "name" }
    *
    * @return table names : table filled with query column names.
    */
    int GetColumnNames(lua_State* L, ElunaQuery* result)
    {
        lua_createtable(L, RESULT->GetFieldCount(), 0);
        int tbl = lua_gettop(L);
        for (size_t i = 0; i < RESULT->GetFieldCount(); ++i)
        {
            Eluna::Push(L, RESULT->GetFieldInfo(i).name);
            lua_rawseti(L, tbl, i + 1);
        }
        lua_settop(L, tbl);
        return 1;
    }
};
#undef RESULT

#endif
