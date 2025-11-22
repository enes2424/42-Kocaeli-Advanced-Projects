package com.example.hangouts

import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.os.Build
import android.telephony.SmsMessage
import androidx.core.app.NotificationCompat
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import android.widget.Toast
import androidx.core.content.ContextCompat
import android.app.ActivityManager
import android.os.Handler
import android.os.Looper

class SmsReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        if (intent.action == "android.provider.Telephony.SMS_RECEIVED") {
            val bundle = intent.extras ?: return
            val pdus = bundle.get("pdus") as? Array<*> ?: return
            val format = bundle.getString("format")

            for (pdu in pdus) {
                try {
                    val sms = SmsMessage.createFromPdu(pdu as ByteArray, format)
                    var sender = sms.displayOriginatingAddress
                    val body = sms.messageBody

                    val dbHelper = DbHelper(context)
                    val db = dbHelper.readableDatabase
                    val query = "SELECT id, name, surname, nickname FROM contacts WHERE phone_number = ?"
                    val cursor = db.rawQuery(query, arrayOf(sender))
                    val contactExists = cursor.count == 1
                    var contactId: Long = -1
                    if (!contactExists) {
                        contactId = MainActivity.addContact(
                            context,
                            name = sender,
                            surname = "",
                            nickname = "",
                            phoneNumber = sender,
                            email = ""
                        )
                    } else {
                        cursor.moveToFirst()
                        contactId = cursor.getLong(cursor.getColumnIndexOrThrow("id"))
                        val name = cursor.getString(cursor.getColumnIndexOrThrow("name"))
                        val surname = cursor.getString(cursor.getColumnIndexOrThrow("surname"))
                        val nickname = cursor.getString(cursor.getColumnIndexOrThrow("nickname"))
                        sender = if (nickname.isNotEmpty()) {
                            nickname
                        } else {
                            "$name $surname"
                        }
                    }
                    cursor.close()
                    db.close()

                    val isAppForeground = isAppInForeground(context)
                    val timestamp = sms.timestampMillis

                    if (isAppForeground) {
                        val intent = Intent(context, MainActivity::class.java)
                        intent.putExtra("id", contactId)
                        intent.putExtra("sender", sender)
                        intent.putExtra("body", body)
                        intent.putExtra("contactExists", contactExists)
                        intent.putExtra("timestamp", timestamp.toString())
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_SINGLE_TOP)
                        context.startActivity(intent)
                    } else {
                        Handler(Looper.getMainLooper()).post {
                            Toast.makeText(context, "$sender: $body", Toast.LENGTH_SHORT).show()
                        }
                    }

                    Thread {
                        try {
                            val db = dbHelper.writableDatabase
                            val insert = "INSERT INTO messages (contact_id, status, body, timestamp) VALUES (?, 'received', ?, ?)"
                            val stmt = db.compileStatement(insert)
                            stmt.bindLong(1, contactId.toLong())
                            stmt.bindString(2, body)
                            stmt.bindString(3, timestamp.toString())
                            stmt.executeInsert()
                            stmt.close()
                            db.close()
                        } catch (e: Exception) {
                            Handler(Looper.getMainLooper()).post {
                                Toast.makeText(context, "Database error: ${e.message}", Toast.LENGTH_LONG).show()
                            }
                        }
                    }.start()
                } catch (e: Exception) {
                    Handler(Looper.getMainLooper()).post {
                        Toast.makeText(context, "Error: ${e.message}", Toast.LENGTH_LONG).show()
                    }
                }
            }
        }
    }

    private fun isAppInForeground(context: Context): Boolean {
        val activityManager = context.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val appProcesses = activityManager.runningAppProcesses ?: return false
        val packageName = context.packageName
        for (appProcess in appProcesses) {
            if (appProcess.processName == packageName &&
                appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
                return true
            }
        }
        return false
    }
}
