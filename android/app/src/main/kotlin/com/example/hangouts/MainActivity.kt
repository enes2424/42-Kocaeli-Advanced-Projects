package com.example.hangouts

import android.Manifest
import android.app.role.RoleManager
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.MediaStore
import android.telephony.SmsManager
import androidx.annotation.RequiresApi
import androidx.core.app.ActivityCompat
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.EventChannel
import io.flutter.plugin.common.MethodChannel
import android.os.Handler
import android.os.Looper
import android.widget.Toast
import android.util.Base64
import android.media.ExifInterface
import android.graphics.Matrix

class MainActivity: FlutterActivity() {
    private val CHANNEL = "com.example.sms"
    private var eventSink: EventChannel.EventSink? = null
    private var pendingResult: MethodChannel.Result? = null
    private var lastBackgroundedTime: Long = System.currentTimeMillis()
    private var dataRefreshSink: EventChannel.EventSink? = null
    private var isFirstResume = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val permissions = mutableListOf(
            Manifest.permission.SEND_SMS,
            Manifest.permission.CALL_PHONE,
            Manifest.permission.RECEIVE_SMS,
            Manifest.permission.READ_SMS,
            Manifest.permission.READ_EXTERNAL_STORAGE
        )

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            permissions.add(Manifest.permission.POST_NOTIFICATIONS)
            permissions.add(Manifest.permission.READ_MEDIA_IMAGES)
        }

        val missing = permissions.filter {
            ActivityCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }

        if (missing.isNotEmpty()) {
            ActivityCompat.requestPermissions(this, missing.toTypedArray(), 101)
        }

    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        val id = intent.getLongExtra("id", -1L).toInt()
        val sender = intent.getStringExtra("sender")
        val body = intent.getStringExtra("body")
        val contactExists = intent.getBooleanExtra("contactExists", true)
        val timestamp = intent.getStringExtra("timestamp")
        if (sender != null && body != null && timestamp != null) {
            eventSink?.success(mapOf("id" to id, "sender" to sender, "body" to body, "contactExists" to contactExists, "timestamp" to timestamp))
        }
    }

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)

        EventChannel(flutterEngine.dartExecutor.binaryMessenger, "smsReceiver").setStreamHandler(
            object : EventChannel.StreamHandler {
                override fun onListen(arguments: Any?, events: EventChannel.EventSink?) {
                    eventSink = events
                }

                override fun onCancel(arguments: Any?) {
                    eventSink = null
                }
            }
        )

        EventChannel(flutterEngine.dartExecutor.binaryMessenger, "dataRefresh").setStreamHandler(
            object : EventChannel.StreamHandler {
                override fun onListen(arguments: Any?, events: EventChannel.EventSink?) {
                    dataRefreshSink = events
                }

                override fun onCancel(arguments: Any?) {
                    dataRefreshSink = null
                }
            }
        )

        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, CHANNEL)
            .setMethodCallHandler { call, result ->
                when(call.method) {
                    "getAllContacts" -> {
                        val contacts = getAllContactsFromDatabase()
                        result.success(contacts)
                    }
                    "addContact" -> {
                        val name = call.argument<String>("name") ?: ""
                        val surname = call.argument<String>("surname") ?: ""
                        val nickname = call.argument<String>("nickname") ?: ""
                        val phoneNumber = call.argument<String>("phoneNumber") ?: ""
                        val email = call.argument<String>("email") ?: ""
                        val id = addContact(this, name, surname, nickname, phoneNumber, email)
                        result.success(id)
                    }
                    "updateContact" -> {
                        val contactId = call.argument<Int>("contactId") ?: 0
                        val name = call.argument<String>("name") ?: ""
                        val surname = call.argument<String>("surname") ?: ""
                        val nickname = call.argument<String>("nickname") ?: ""
                        val phoneNumber = call.argument<String>("phoneNumber") ?: ""
                        val email = call.argument<String>("email") ?: ""
                        val profilePicture = call.argument<String>("profilePicture") ?: ""
                        updateContact(contactId, name, surname, nickname, phoneNumber, email, profilePicture)
                        result.success(null)
                    }
                    "deleteContact" -> {
                        val contactId = call.argument<Int>("contactId") ?: 0
                        deleteContact(contactId)
                        result.success(null)
                    }
                    "getAllMessages" -> {
                        val messages = getAllMessagesFromDatabase()
                        result.success(messages)
                    }
                    "sendSMS" -> {
                        val contactId = call.argument<Int>("contactId") ?: 0
                        val phone = call.argument<String>("phoneNumber") ?: ""
                        val msg = call.argument<String>("message") ?: ""
                        sendSMS(contactId, phone, msg, result)
                    }
                    "pickImage" -> {
                        val intent = Intent(Intent.ACTION_GET_CONTENT).apply {
                            type = "image/*"
                        }
                        pendingResult = result
                        startActivityForResult(intent, 1001)
                    }
                    "getHeaderColor" -> {
                        val color = getHeaderColor()
                        result.success(color)
                    }
                    "setHeaderColor" -> {
                        val red = call.argument<Int>("red") ?: 63
                        val green = call.argument<Int>("green") ?: 81
                        val blue = call.argument<Int>("blue") ?: 181
                        setHeaderColor(red, green, blue)
                        result.success(null)
                    }
                    "makeCall" -> {
                        val phoneNumber = call.argument<String>("phoneNumber")
                        if (phoneNumber != null) {
                            makeCall(phoneNumber, result)
                        } else {
                            result.error("INVALID_ARGS", "Phone number is required", null)
                        }
                    }
                    else -> result.notImplemented()
                }
            }
    }

    private fun getAllContactsFromDatabase(): List<Map<String, Any>> {
        val dbHelper = DbHelper(this)
        val db = dbHelper.readableDatabase
        val cursor = db.rawQuery("SELECT id, name, surname, nickname, phone_number, email, profile_picture FROM contacts", null)
        val list = mutableListOf<Map<String, Any>>()
        while (cursor.moveToNext()) {
            val map = mapOf(
                "id" to cursor.getInt(0),
                "name" to cursor.getString(1),
                "surname" to cursor.getString(2),
                "nickname" to cursor.getString(3),
                "phone_number" to cursor.getString(4),
                "email" to cursor.getString(5),
                "profile_picture" to cursor.getString(6)
            )
            list.add(map)
        }
        cursor.close()
        db.close()
        return list
    }

    companion object {
        fun addContact(
            context: Context,
            name: String,
            surname: String,
            nickname: String,
            phoneNumber: String,
            email: String,
        ) : Long {
            val dbHelper = DbHelper(context)
            val db = dbHelper.writableDatabase
            val insert = "INSERT INTO contacts (name, surname, nickname, phone_number, email, profile_picture) VALUES (?, ?, ?, ?, ?, '')"
            val stmt = db.compileStatement(insert)
            stmt.bindString(1, name)
            stmt.bindString(2, surname)
            stmt.bindString(3, nickname)
            stmt.bindString(4, phoneNumber)
            stmt.bindString(5, email)
            val id = stmt.executeInsert()
            stmt.close()
            db.close()
            return id
        }
    }

    private fun updateContact(
        contactId: Int,
        name: String,
        surname: String,
        nickname: String,
        phoneNumber: String,
        email: String,
        profilePicture: String
    ) {
        try {
            val dbHelper = DbHelper(this)
            val db = dbHelper.writableDatabase
            val update = "UPDATE contacts SET name = ?, surname = ?, nickname = ?, phone_number = ?, email = ?, profile_picture = ? WHERE id = ?"
            val stmt = db.compileStatement(update)
            stmt.bindString(1, name)
            stmt.bindString(2, surname)
            stmt.bindString(3, nickname)
            stmt.bindString(4, phoneNumber)
            stmt.bindString(5, email)
            stmt.bindString(6, profilePicture)
            stmt.bindLong(7, contactId.toLong())
            stmt.executeUpdateDelete()
            stmt.close()
            db.close()
        } catch (e: Exception) {
            Handler(Looper.getMainLooper()).post {
                Toast.makeText(this, "Database error: ${e.message}", Toast.LENGTH_LONG).show()
            }
        }
    }

    private fun deleteContact(contactId: Int) {
        try {
            val dbHelper = DbHelper(this)
            val db = dbHelper.writableDatabase
            val delete = "DELETE FROM contacts WHERE id = ?"
            val stmt = db.compileStatement(delete)
            stmt.bindLong(1, contactId.toLong())
            val delete2 = "DELETE FROM messages WHERE contact_id = ?"
            val stmt2 = db.compileStatement(delete2)
            stmt2.bindLong(1, contactId.toLong())
            stmt2.executeUpdateDelete()
            stmt.executeUpdateDelete()
            stmt.close()
            stmt2.close()
            db.close()
        } catch (e: Exception) {
            Handler(Looper.getMainLooper()).post {
                Toast.makeText(this, "Database error: ${e.message}", Toast.LENGTH_LONG).show()
            }
        }
    }

    private fun sendSMS(contactId: Int, phoneNumber: String, message: String, result: MethodChannel.Result) {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.SEND_SMS) != PackageManager.PERMISSION_GRANTED) {
            result.error("PERMISSION_DENIED", "SMS permission denied, please enable in app settings", null)
        } else {
            val smsManager = SmsManager.getDefault()
            val parts = smsManager.divideMessage(message)
            smsManager.sendMultipartTextMessage(phoneNumber, null, parts, null, null)
            val timestamp = System.currentTimeMillis()

            Thread {
                try {
                    val dbHelper = DbHelper(this)
                    val db = dbHelper.writableDatabase
                    val insert = "INSERT INTO messages (contact_id, status, body, timestamp) VALUES (?, 'sent', ?, ?)"
                    val stmt = db.compileStatement(insert)
                    stmt.bindLong(1, contactId.toLong())
                    stmt.bindString(2, message)
                    stmt.bindString(3, timestamp.toString())
                    stmt.executeInsert()
                    stmt.close()
                    db.close()
                } catch (e: Exception) {
                    Handler(Looper.getMainLooper()).post {
                        Toast.makeText(this, "Database error: ${e.message}", Toast.LENGTH_LONG).show()
                    }
                }
            }.start()

            /*Thread {
                try {
                    val dbHelper = DbHelper(this)
                    val db = dbHelper.writableDatabase
                    val insert =  "INSERT INTO contacts (name, surname, nickname, phone_number, email, profile_picture) VALUES ('TURKCELL', '', '', 'TURKCELL', '', '')"
                    val stmt = db.compileStatement(insert)
                    stmt.executeInsert()
                    stmt.close()
                    db.close()
                } catch (e: Exception) {
                    Handler(Looper.getMainLooper()).post {
                        Toast.makeText(this, "Database error: ${e.message}", Toast.LENGTH_LONG).show()
                    }
                }
            }.start()*/

            result.success(timestamp.toString())
        }
    }

    private fun makeCall(phoneNumber: String, result: MethodChannel.Result) {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED) {
            result.error("PERMISSION_DENIED", "Call permission denied, please enable in app settings", null)
        } else {
            val intent = Intent(Intent.ACTION_CALL).apply {
                data = Uri.parse("tel:$phoneNumber")
            }
            startActivity(intent)
            result.success(null)
        }
    }

    private fun getAllMessagesFromDatabase(): List<Map<String, Any>> {
        val dbHelper = DbHelper(this)
        val db = dbHelper.readableDatabase
        val cursor = db.rawQuery("SELECT contact_id, status, body, timestamp FROM messages", null)
        val list = mutableListOf<Map<String, Any>>()
        while (cursor.moveToNext()) {
            val map = mapOf(
                "contact_id" to cursor.getInt(0),
                "status" to cursor.getString(1),
                "body" to cursor.getString(2),
                "timestamp" to cursor.getString(3)
            )
            list.add(map)
        }
        cursor.close()
        db.close()
        return list
    }

    private fun getHeaderColor(): Map<String, Int> {
        val dbHelper = DbHelper(this)
        val db = dbHelper.readableDatabase
        val cursor = db.rawQuery("SELECT red, green, blue FROM header_colors", null)
        val headerColor = mutableMapOf<String, Int>()
        if (cursor.moveToFirst()) {
            headerColor["red"] = cursor.getInt(0)
            headerColor["green"] = cursor.getInt(1)
            headerColor["blue"] = cursor.getInt(2)
        }
        cursor.close()
        db.close()
        return headerColor
    }

    private fun setHeaderColor(red: Int, green: Int, blue: Int) {
        val dbHelper = DbHelper(this)
        val db = dbHelper.writableDatabase
        val update = "UPDATE header_colors SET red = ?, green = ?, blue = ?"
        val stmt = db.compileStatement(update)
        stmt.bindLong(1, red.toLong())
        stmt.bindLong(2, green.toLong())
        stmt.bindLong(3, blue.toLong())
        stmt.executeUpdateDelete()
        stmt.close()
        db.close()
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 1001 && resultCode == RESULT_OK) {
            val uri = data?.data
            if (uri != null) {
                val base64 = getResizedImageBase64(uri)
                pendingResult?.success(base64)
                pendingResult = null
            } else {
                pendingResult?.error("NO_URI", "No URI selected", null)
                pendingResult = null
            }
        } else if (requestCode == 1001) {
            pendingResult?.error("CANCELLED", "Image selection cancelled", null)
            pendingResult = null
        }
    }

    private fun getResizedImageBase64(uri: Uri): String {
        val inputStream = contentResolver.openInputStream(uri)
        val originalBitmap = BitmapFactory.decodeStream(inputStream)
        inputStream?.close()

        val exifInputStream = contentResolver.openInputStream(uri)
        val exif = ExifInterface(exifInputStream!!)
        exifInputStream.close()
        val orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL)

        val rotatedBitmap = when (orientation) {
            ExifInterface.ORIENTATION_ROTATE_90 -> rotateBitmap(originalBitmap, 90f)
            ExifInterface.ORIENTATION_ROTATE_180 -> rotateBitmap(originalBitmap, 180f)
            ExifInterface.ORIENTATION_ROTATE_270 -> rotateBitmap(originalBitmap, 270f)
            else -> originalBitmap
        }

        if (rotatedBitmap !== originalBitmap) {
            originalBitmap.recycle()
        }

        val maxSize = 1000
        val ratio = minOf(maxSize.toFloat() / rotatedBitmap.width, maxSize.toFloat() / rotatedBitmap.height)
        val newWidth = (rotatedBitmap.width * ratio).toInt()
        val newHeight = (rotatedBitmap.height * ratio).toInt()

        val resizedBitmap = Bitmap.createScaledBitmap(rotatedBitmap, newWidth, newHeight, true)
        rotatedBitmap.recycle()

        val outputStream = java.io.ByteArrayOutputStream()
        resizedBitmap.compress(Bitmap.CompressFormat.JPEG, 80, outputStream)
        resizedBitmap.recycle()

        val bytes = outputStream.toByteArray()
        outputStream.close()

        return Base64.encodeToString(bytes, Base64.NO_WRAP)
    }

    private fun rotateBitmap(bitmap: Bitmap, degrees: Float): Bitmap {
        val matrix = Matrix().apply { postRotate(degrees) }
        return Bitmap.createBitmap(bitmap, 0, 0, bitmap.width, bitmap.height, matrix, true)
    }

    override fun onPause() {
        super.onPause()
        lastBackgroundedTime = System.currentTimeMillis()
    }

    override fun onResume() {
        super.onResume()
        if (!isFirstResume) {
            val currentTime = System.currentTimeMillis()
            val timeInBackground = currentTime - lastBackgroundedTime
            if (timeInBackground >= 1000) {
                val seconds = (timeInBackground / 1000) % 60
                val minutes = (timeInBackground / (1000 * 60)) % 60
                val hours = (timeInBackground / (1000 * 60 * 60))

                val message = when {
                    hours > 0 -> "App was backgrounded for: ${hours}h ${minutes}m ${seconds}s"
                    minutes > 0 -> "App was backgrounded for: ${minutes}m ${seconds}s"
                    else -> "App was backgrounded for: ${seconds}s"
                }

                Toast.makeText(this, message, Toast.LENGTH_LONG).show()
            }
            dataRefreshSink?.success("refresh")
        }
        isFirstResume = false
    }
}
