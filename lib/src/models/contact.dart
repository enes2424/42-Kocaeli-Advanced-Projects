import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'dart:convert';

import '../widgets/snackbar.dart';

import '../sms/sms_helper.dart';

import '../pages/info_page.dart';
import '../pages/home_page.dart';
import '../pages/chat_page.dart';

import '../models/app_words.dart';
import '../models/sms.dart';

class Contact {
  final int id;
  String name;
  String surname;
  String nickname;
  String phoneNumber;
  String email;
  String profilePicture;

  final List<SmsMessage> _messages = [];

  List<SmsMessage> get messages => _messages;

  Contact({
    required this.id,
    required this.name,
    required this.surname,
    required this.nickname,
    required this.phoneNumber,
    required this.email,
    required this.profilePicture,
  });

  void addMessage(SmsMessage message) {
    _messages.add(message);
  }

  SmsMessage? _getLastMessage() {
    if (_messages.isEmpty) {
      return null;
    }
    return _messages.last;
  }

  Widget profileAvatar() {
    if (profilePicture.isEmpty) {
      return SizedBox(
        width: 100,
        height: 100,
        child: CircleAvatar(
          backgroundColor: homePageKey.currentState!.headerColor.withValues(
            red: homePageKey.currentState!.headerColor.r * 0.8,
            green: homePageKey.currentState!.headerColor.g * 0.8,
            blue: homePageKey.currentState!.headerColor.b * 0.8,
          ),
          child: Text(
            nickname.isEmpty
                ? name == phoneNumber
                      ? '?'
                      : name[0].toUpperCase()
                : nickname[0].toUpperCase(),
            style: const TextStyle(
              fontSize: 40,
              color: Colors.white,
              fontWeight: FontWeight.bold,
            ),
          ),
        ),
      );
    } else {
      return Image.memory(base64Decode(profilePicture), fit: BoxFit.contain);
    }
  }

  Future<void> update({
    String? name,
    String? surname,
    String? nickname,
    String? phoneNumber,
    String? email,
    String? profilePicture,
  }) async {
    final language = homePageKey.currentState!.appLanguage;

    try {
      await _channel.invokeMethod('updateContact', {
        'contactId': id,
        'name': name ?? this.name,
        'surname': surname ?? this.surname,
        'nickname': nickname ?? this.nickname,
        'phoneNumber': phoneNumber ?? this.phoneNumber,
        'email': email ?? this.email,
        'profilePicture': profilePicture ?? this.profilePicture,
      });
    } on PlatformException catch (e) {
      showCustomSnackBar(
        // ignore: use_build_context_synchronously
        contextHolder,
        '${appWords[language]!['update_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
      return;
    }

    if (name != null) this.name = name;
    if (surname != null) this.surname = surname;
    if (nickname != null) this.nickname = nickname;
    if (phoneNumber != null) this.phoneNumber = phoneNumber;
    if (email != null) this.email = email;
    if (profilePicture != null) {
      this.profilePicture = profilePicture;
    }

    _contacts.sort(
      (a, b) =>
          a.name == a.phoneNumber &&
              ((a.name.startsWith('+90') && !b.name.startsWith('+90')) ||
                  b.name != b.phoneNumber)
          ? 1
          : b.name == b.phoneNumber
          ? -1
          : (a.nickname.isEmpty ? "${a.name} ${a.surname}" : a.nickname)
                .toLowerCase()
                .compareTo(
                  (b.nickname.isEmpty ? "${b.name} ${b.surname}" : b.nickname)
                      .toLowerCase(),
                ),
    );

    showCustomSnackBar(
      // ignore: use_build_context_synchronously
      contextHolder,
      appWords[language]!['update_success']!,
      type: SnackBarType.success,
    );
  }

  Future<void> delete() async {
    final language = homePageKey.currentState!.appLanguage;

    try {
      await _channel.invokeMethod('deleteContact', {'contactId': id});
    } on PlatformException catch (e) {
      showCustomSnackBar(
        // ignore: use_build_context_synchronously
        contextHolder,
        '${appWords[language]!['delete_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
    }

    _contacts.removeWhere((contact) => contact.id == id);

    showCustomSnackBar(
      // ignore: use_build_context_synchronously
      contextHolder,
      appWords[language]!['delete_success']!,
      type: SnackBarType.success,
    );
  }

  static const MethodChannel _channel = MethodChannel('com.example.sms');

  static const MethodChannel channel = _channel;

  static List<Contact> _contacts = [];

  static late BuildContext contextHolder;

  static int get count => _contacts.length;

  static Future<void> getAllContacts(BuildContext context) async {
    try {
      contextHolder = context;

      final List<dynamic> contacts = await _channel.invokeMethod(
        'getAllContacts',
      );

      final contactsList =
          contacts.map((msg) {
            final contact = getById(msg['id']);

            if (contact != null) {
              contact.name = msg['name'];
              contact.surname = msg['surname'];
              contact.nickname = msg['nickname'];
              contact.phoneNumber = msg['phone_number'];
              contact.email = msg['email'];
              contact.profilePicture = msg['profile_picture'];
              return contact;
            }

            return Contact(
              id: msg['id'],
              name: msg['name'],
              surname: msg['surname'],
              nickname: msg['nickname'],
              phoneNumber: msg['phone_number'],
              email: msg['email'],
              profilePicture: msg['profile_picture'],
            );
          }).toList()..sort(
            (a, b) =>
                a.name == a.phoneNumber &&
                    ((a.name.startsWith('+90') && !b.name.startsWith('+90')) ||
                        b.name != b.phoneNumber)
                ? 1
                : b.name == b.phoneNumber
                ? -1
                : (a.nickname.isEmpty ? "${a.name} ${a.surname}" : a.nickname)
                      .toLowerCase()
                      .compareTo(
                        (b.nickname.isEmpty
                                ? "${b.name} ${b.surname}"
                                : b.nickname)
                            .toLowerCase(),
                      ),
          );

      _contacts = contactsList;
    } on PlatformException catch (e) {
      showCustomSnackBar(
        // ignore: use_build_context_synchronously
        context,
        '${appWords[homePageKey.currentState!.appLanguage]!['load_contacts_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
    }

    if (context.mounted) {
      final messages = await SmsHelper.getAllMessages(context);

      messages.sort((a, b) => a.timestamp.compareTo(b.timestamp));

      Contact._clearMessages();

      for (final message in messages) {
        final contact = getById(message.contactId);
        contact?.addMessage(message);
      }
    }
  }

  static void _clearMessages() {
    for (final contact in _contacts) {
      contact._messages.clear();
    }
  }

  static Contact? getById(int id) {
    for (final contact in _contacts) {
      if (contact.id == id) {
        return contact;
      }
    }
    return null;
  }

  static bool nicknameExists(String nickname) {
    for (final contact in _contacts) {
      if (contact.nickname.toLowerCase() == nickname.toLowerCase()) {
        return true;
      }
    }
    return false;
  }

  static bool phoneExists(String phoneNumber) {
    for (final contact in _contacts) {
      if (contact.phoneNumber == phoneNumber) {
        return true;
      }
    }
    return false;
  }

  static Future<void> addContact(
    BuildContext context,
    Map<String, dynamic> contactData,
  ) async {
    try {
      final id =
          contactData['id'] ??
          await _channel.invokeMethod('addContact', contactData);
      _contacts.add(
        Contact(
          id: id,
          name: contactData['name']!,
          surname: contactData['surname']!,
          nickname: contactData['nickname']!,
          phoneNumber: contactData['phoneNumber']!,
          email: contactData['email']!,
          profilePicture: '',
        ),
      );
      _contacts.sort(
        (a, b) =>
            a.name == a.phoneNumber &&
                ((a.name.startsWith('+90') && !b.name.startsWith('+90')) ||
                    b.name != b.phoneNumber)
            ? 1
            : b.name == b.phoneNumber
            ? -1
            : (a.nickname.isEmpty ? "${a.name} ${a.surname}" : a.nickname)
                  .toLowerCase()
                  .compareTo(
                    (b.nickname.isEmpty ? "${b.name} ${b.surname}" : b.nickname)
                        .toLowerCase(),
                  ),
      );
    } on PlatformException catch (e) {
      showCustomSnackBar(
        //ignore: use_build_context_synchronously
        context,
        '${appWords[homePageKey.currentState!.appLanguage]!['add_error']!}: ${e.message}',
        type: SnackBarType.error,
      );
    }
  }

  static Widget card(
    BuildContext context,
    int index, {
    required bool isExpanded,
    required ValueChanged<bool> onExpansionChanged,
  }) {
    final language = homePageKey.currentState!.appLanguage;

    final contact = _contacts[index];

    final displayPhoneNumber = contact.phoneNumber.startsWith('+90')
        ? "+90 ${contact.phoneNumber.substring(3, 6)} ${contact.phoneNumber.substring(6, 9)} "
              "${contact.phoneNumber.substring(9, 11)} ${contact.phoneNumber.substring(11)}"
        : contact.phoneNumber;

    final displayText = contact.nickname.isNotEmpty
        ? contact.nickname
        : (contact.name != contact.phoneNumber
              ? "${contact.name} ${contact.surname}"
              : displayPhoneNumber);

    final lastMessage = contact._getLastMessage();

    final datetime = lastMessage != null
        ? DateTime.fromMillisecondsSinceEpoch(int.parse(lastMessage.timestamp))
        : null;

    return Card(
      elevation: 4,
      margin: const EdgeInsets.only(bottom: 12),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      clipBehavior: Clip.antiAlias,
      child: Theme(
        data: Theme.of(context).copyWith(dividerColor: Colors.transparent),
        child: ExpansionTile(
          key: ValueKey('exp_${index}_$isExpanded'),
          maintainState: true,
          initiallyExpanded: isExpanded,
          onExpansionChanged: onExpansionChanged,
          tilePadding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
          childrenPadding: const EdgeInsets.symmetric(
            horizontal: 16,
            vertical: 8,
          ),
          leading: SizedBox(
            height: 48,
            width: 48,
            child: FittedBox(
              fit: BoxFit.contain,
              child: contact.profileAvatar(),
            ),
          ),
          title: Text(
            displayText,
            overflow: TextOverflow.ellipsis,
            style: const TextStyle(fontWeight: FontWeight.bold),
          ),
          subtitle: lastMessage != null
              ? Text(
                  '${lastMessage.status == "sent" ? "${appWords[language]!['me']!}: " : ""}${lastMessage.body}',
                  maxLines: 1,
                  overflow: TextOverflow.ellipsis,
                  style: TextStyle(color: Colors.grey[600]),
                )
              : null,
          trailing: lastMessage != null
              ? Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text(
                      '${datetime!.day}/${datetime.month}/${datetime.year}',
                      style: TextStyle(color: Colors.grey[600], fontSize: 12),
                    ),
                    Text(
                      '${datetime.hour}:${datetime.minute.toString().padLeft(2, '0')}',
                      style: TextStyle(color: Colors.grey[600], fontSize: 12),
                    ),
                  ],
                )
              : const SizedBox.shrink(),
          children: [
            if (displayPhoneNumber != displayText)
              Align(
                alignment: Alignment.centerLeft,
                child: Padding(
                  padding: const EdgeInsets.only(bottom: 8.0, left: 20.0),
                  child: Text(
                    "${appWords[language]!['phone']!}: $displayPhoneNumber",
                    style: TextStyle(color: Colors.grey[700]),
                  ),
                ),
              ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                if (displayPhoneNumber.startsWith('+90'))
                  TextButton.icon(
                    onPressed: () async {
                      try {
                        await _channel.invokeMethod('makeCall', {
                          'phoneNumber': contact.phoneNumber,
                        });
                      } on PlatformException catch (e) {
                        showCustomSnackBar(
                          // ignore: use_build_context_synchronously
                          context,
                          '${appWords[language]!['call_error']!}: ${e.message}',
                          type: SnackBarType.error,
                        );
                      }
                    },
                    icon: const Icon(Icons.call, color: Colors.green),
                    label: Text(appWords[language]!['call']!),
                  ),
                TextButton.icon(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) =>
                            ChatPage(key: chatPageKey, contact: contact),
                      ),
                    );
                  },
                  icon: const Icon(Icons.message, color: Colors.orange),
                  label: Text(appWords[language]!['message']!),
                ),
                TextButton.icon(
                  onPressed: () {
                    Future<void>? future = Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => InfoPage(contact: contact),
                      ),
                    );

                    homePageKey.currentState?.refresh(future);
                  },
                  icon: const Icon(Icons.info, color: Colors.blue),
                  label: Text(appWords[language]!['info']!),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
