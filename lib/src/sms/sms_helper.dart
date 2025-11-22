import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import '../widgets/snackbar.dart';

import '../models/app_words.dart';
import '../models/sms.dart';

import '../pages/home_page.dart';

class SmsHelper {
  static const MethodChannel _channel = MethodChannel('com.example.sms');

  static Future<String?> sendSms(
    BuildContext context,
    int contactId,
    String phoneNumber,
    String message,
  ) async {
    try {
      final String timestamp = await _channel.invokeMethod('sendSMS', {
        'contactId': contactId,
        'phoneNumber': phoneNumber,
        'message': message,
      });

      return timestamp;
    } on PlatformException catch (e) {
      showCustomSnackBar(
        // ignore: use_build_context_synchronously
        context,
        '${appWords[homePageKey.currentState!.appLanguage]!['send_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
      return null;
    }
  }

  static Future<List<SmsMessage>> getAllMessages(BuildContext context) async {
    try {
      final List<dynamic> messages = await _channel.invokeMethod(
        'getAllMessages',
      );
      return messages
          .map(
            (msg) => SmsMessage(
              contactId: msg['contact_id'],
              status: msg['status'],
              body: msg['body'],
              timestamp: msg['timestamp'],
            ),
          )
          .toList()
        ..sort((a, b) => b.timestamp.compareTo(a.timestamp));
    } on PlatformException catch (e) {
      showCustomSnackBar(
        // ignore: use_build_context_synchronously
        context,
        '${appWords[homePageKey.currentState!.appLanguage]!['load_messages_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
      return [];
    }
  }
}
