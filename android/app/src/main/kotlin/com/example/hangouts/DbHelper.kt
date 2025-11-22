package com.example.hangouts

import android.content.Context
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper

class DbHelper(context: Context) :
    SQLiteOpenHelper(context, DATABASE_NAME, null, DATABASE_VERSION) {

    companion object {
        private const val DATABASE_NAME = "sms_db"
        private const val DATABASE_VERSION = 1
    }

    override fun onCreate(db: SQLiteDatabase) {
        val createContactsTable = """
            CREATE TABLE contacts (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT,
                surname TEXT,
                nickname TEXT,
                phone_number TEXT,
                email TEXT,
                profile_picture TEXT
            );
        """.trimIndent()
        val createMessagesTable = """
            CREATE TABLE messages (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                status TEXT,
                body TEXT,
                timestamp TEXT,
                contact_id INTEGER,
                FOREIGN KEY (contact_id) REFERENCES contacts(id)
            );
        """.trimIndent()
        val createHeaderColorTable = """
            CREATE TABLE header_colors (
                red INTEGER,
                green INTEGER,
                blue INTEGER
            );
        """.trimIndent()

        val defaultColorInsert = """
            INSERT INTO header_colors (red, green, blue) VALUES (63, 81, 181);
        """.trimIndent()
        db.execSQL(createContactsTable)
        db.execSQL(createMessagesTable)
        db.execSQL(createHeaderColorTable)
        db.execSQL(defaultColorInsert)
    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS messages")
        db.execSQL("DROP TABLE IF EXISTS contacts")
        db.execSQL("DROP TABLE IF EXISTS header_colors")
        onCreate(db)
    }
}
