import 'package:flutter/material.dart';

import '../widgets/show_dialog.dart';

import '../models/app_words.dart';
import '../models/contact.dart';

import 'home_page.dart';

class InfoPage extends StatefulWidget {
  final Contact contact;

  const InfoPage({super.key, required this.contact});

  @override
  State<InfoPage> createState() => _InfoPageState();
}

class _InfoPageState extends State<InfoPage> {
  Contact get contact => widget.contact;
  String get language => homePageKey.currentState!.appLanguage;

  bool isLoading = false;

  void safeSetState(VoidCallback fn) {
    if (mounted) {
      setState(fn);
    }
  }

  @override
  Widget build(BuildContext context) {
    final hideEditButton = !contact.phoneNumber.startsWith('+90');

    return isLoading
        ? Scaffold(
            body: const Center(
              child: SizedBox(
                width: 100,
                height: 100,
                child: CircularProgressIndicator(
                  color: Colors.indigo,
                  strokeWidth: 8,
                  strokeCap: StrokeCap.round,
                ),
              ),
            ),
          )
        : Scaffold(
            appBar: AppBar(
              title: Text(appWords[language]!['contact_info']!),
              backgroundColor: homePageKey.currentState!.headerColor,
              foregroundColor: Colors.white,
            ),
            body: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 20.0),
              child: SafeArea(
                child: ListView(
                  children: [
                    const SizedBox(height: 20),
                    contact.profileAvatar(),
                    const SizedBox(height: 20),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        IconButton(
                          icon: const Icon(Icons.edit),
                          onPressed: () async {
                            final base64 = await Contact.channel.invokeMethod(
                              'pickImage',
                            );
                            if (base64 != null) {
                              safeSetState(() => isLoading = true);
                              await contact.update(profilePicture: base64);
                              safeSetState(() => isLoading = false);
                            }
                          },
                        ),
                        const SizedBox(width: 10),
                        IconButton(
                          icon: const Icon(Icons.delete),
                          onPressed: () async {
                            safeSetState(() => isLoading = true);
                            await contact.update(profilePicture: '');
                            safeSetState(() => isLoading = false);
                          },
                        ),
                      ],
                    ),
                    const SizedBox(height: 30),
                    _buildInfoRow(
                      appWords[language]!['name']!,
                      contact.name.isEmpty ? 'absent' : contact.name,
                      onPressed: !hideEditButton
                          ? () async {
                              final name = await showEditNameDialog(
                                context,
                                contact.name,
                              );
                              if (name == null || !context.mounted) return;
                              if (contact.surname.isEmpty) {
                                final surname = await showEditSurnameDialog(
                                  context,
                                  contact.surname,
                                );
                                if (surname == null) return;
                                safeSetState(() => isLoading = true);
                                await contact.update(surname: surname);
                              } else {
                                safeSetState(() => isLoading = true);
                              }
                              await contact.update(name: name);
                              safeSetState(() => isLoading = false);
                            }
                          : null,
                      hide: hideEditButton,
                    ),
                    _buildInfoRow(
                      appWords[language]!['surname']!,
                      contact.surname.isEmpty ? 'absent' : contact.surname,
                      onPressed: !hideEditButton
                          ? () async {
                              final surname = await showEditSurnameDialog(
                                context,
                                contact.surname,
                              );
                              if (surname == null) return;
                              safeSetState(() => isLoading = true);
                              await contact.update(surname: surname);
                              safeSetState(() => isLoading = false);
                            }
                          : null,
                      hide: hideEditButton,
                    ),
                    _buildInfoRow(
                      appWords[language]!['nickname']!,
                      contact.nickname.isEmpty ? 'absent' : contact.nickname,
                      onPressed: !hideEditButton
                          ? () async {
                              final nickname = await showEditNicknameDialog(
                                context,
                                contact.nickname,
                              );
                              if (nickname == null) return;
                              safeSetState(() => isLoading = true);
                              await contact.update(nickname: nickname);
                              safeSetState(() => isLoading = false);
                            }
                          : null,
                      hide: hideEditButton,
                    ),
                    _buildInfoRow(
                      appWords[language]!['phone_number']!,
                      contact.phoneNumber.isEmpty
                          ? 'absent'
                          : !hideEditButton
                          ? "+90 ${contact.phoneNumber.substring(3, 6)} ${contact.phoneNumber.substring(6, 9)} "
                                "${contact.phoneNumber.substring(9, 11)} ${contact.phoneNumber.substring(11)}"
                          : contact.phoneNumber,
                      onPressed: !hideEditButton
                          ? () async {
                              final phoneNumber =
                                  await showEditPhoneNumberDialog(
                                    context,
                                    contact.phoneNumber.substring(3),
                                  );
                              if (phoneNumber == null) return;
                              safeSetState(() => isLoading = true);
                              await contact.update(phoneNumber: phoneNumber);
                              safeSetState(() => isLoading = false);
                            }
                          : null,
                      hide: hideEditButton,
                    ),
                    _buildInfoRow(
                      appWords[language]!['email']!,
                      contact.email.isEmpty ? 'absent' : contact.email,
                      onPressed: () async {
                        final email = await showEditEmailDialog(
                          context,
                          contact.email,
                        );
                        if (email == null) return;
                        safeSetState(() => isLoading = true);
                        await contact.update(email: email);
                        safeSetState(() => isLoading = false);
                      },
                    ),
                    SizedBox(height: 20),
                    Center(
                      child: IconButton(
                        iconSize: 30,
                        icon: const Icon(Icons.delete, color: Colors.red),
                        onPressed: () {
                          Navigator.of(context).pop(contact.delete());
                        },
                      ),
                    ),
                  ],
                ),
              ),
            ),
          );
  }

  Widget _buildInfoRow(
    String label,
    String value, {
    required void Function()? onPressed,
    bool hide = false,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 8.0),
      child: Row(
        children: [
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  label,
                  style: const TextStyle(
                    fontWeight: FontWeight.bold,
                    fontSize: 16,
                  ),
                ),
                Text(
                  value,
                  style: const TextStyle(fontSize: 14, color: Colors.grey),
                ),
              ],
            ),
          ),
          if (!hide)
            IconButton(icon: const Icon(Icons.edit), onPressed: onPressed),
        ],
      ),
    );
  }
}
