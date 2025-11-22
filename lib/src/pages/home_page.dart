import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'dart:async';

import '../widgets/show_dialog.dart';
import '../widgets/snackbar.dart';

import '../models/app_words.dart';
import '../models/contact.dart';
import '../models/sms.dart';

import 'chat_page.dart';

final GlobalKey<HomePageState> homePageKey = GlobalKey<HomePageState>();

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => HomePageState();
}

class HomePageState extends State<HomePage> {
  static const EventChannel _eventChannel = EventChannel('smsReceiver');
  static const EventChannel _dataRefreshChannel = EventChannel('dataRefresh');

  static const MethodChannel _channel = MethodChannel('com.example.sms');

  late StreamSubscription _smsSubscription;
  late StreamSubscription _dataRefreshSubscription;

  late String _appLanguage;

  String get appLanguage => _appLanguage;

  Color _headerColor = Colors.indigo;

  Color get headerColor => _headerColor;

  late Future<void> _loading;

  int _expandedIndex = -1;

  Future<Color> getHeaderColor() async {
    try {
      final dynamic rgb = await _channel.invokeMethod('getHeaderColor');

      return Color.fromARGB(255, rgb['red']!, rgb['green']!, rgb['blue']!);
    } catch (e) {
      return Colors.indigo;
    }
  }

  Future<void> setHeaderColor(Color color) async {
    try {
      await MethodChannel('com.example.sms').invokeMethod('setHeaderColor', {
        'red': (color.r * 255).toInt(),
        'green': (color.g * 255).toInt(),
        'blue': (color.b * 255).toInt(),
      });

      safeSetState(() {
        _headerColor = color;
      });
    } catch (_) {}
  }

  @override
  void initState() {
    super.initState();

    final locale = WidgetsBinding.instance.platformDispatcher.locale;
    _appLanguage = locale.languageCode;

    if (_appLanguage != 'en' && _appLanguage != 'tr') {
      _appLanguage = 'en';
    }

    _loading = () async {
      while (true) {
        await Future.delayed(const Duration(seconds: 1));
      }
    }();

    WidgetsBinding.instance.addPostFrameCallback((_) async {
      _loading = Contact.getAllContacts(context);

      _headerColor = await getHeaderColor();

      safeSetState(() {});

      await _loading;

      _smsSubscription = _eventChannel.receiveBroadcastStream().listen((
        dynamic event,
      ) {
        if (event is Map) {
          final id = event['id'] as int;
          final sender = event['sender'] as String;
          final body = event['body'] as String;
          final contactExists = event['contactExists'] as bool;
          final timestamp = event['timestamp'] as String;

          if (chatPageKey.currentState?.id != id) {
            showCustomSnackBar(
              // ignore: use_build_context_synchronously
              context,
              '$sender: $body',
              type: SnackBarType.info,
            );
          }

          if (mounted && !contactExists && !Contact.phoneExists(sender)) {
            _loading = Contact.addContact(context, {
              'id': id,
              'name': sender,
              'surname': '',
              'nickname': '',
              'phoneNumber': sender,
              'email': '',
            });
          }

          final contact = Contact.getById(id);

          if (contact != null) {
            safeSetState(() {
              contact.addMessage(
                SmsMessage(
                  contactId: id,
                  status: 'received',
                  body: body,
                  timestamp: timestamp,
                ),
              );
              chatPageKey.currentState?.safeSetState(() {});
              if (chatPageKey.currentState?.id == id) {
                chatPageKey.currentState?.scrollToBottomOnNewMessage();
              }
            });
          }
        }
      });

      _dataRefreshSubscription = _dataRefreshChannel
          .receiveBroadcastStream()
          .listen((event) async {
            if (event == "refresh" && mounted) {
              refresh(Contact.getAllContacts(context));

              await _loading;

              chatPageKey.currentState?.safeSetState(() {});
            }
          });
    });
  }

  @override
  void dispose() {
    _smsSubscription.cancel();
    _dataRefreshSubscription.cancel();
    super.dispose();
  }

  void safeSetState(VoidCallback fn) {
    if (mounted) setState(fn);
  }

  void refresh(Future<void>? future) {
    if (future == null) return;
    safeSetState(() {
      _loading = future;
    });
  }

  @override
  Widget build(BuildContext context) => FutureBuilder<void>(
    future: _loading,
    builder: (context, snapshot) {
      if (snapshot.connectionState == ConnectionState.waiting) {
        return Scaffold(
          body: Center(
            child: SizedBox(
              width: 100,
              height: 100,
              child: CircularProgressIndicator(
                color: _headerColor,
                strokeWidth: 8,
                strokeCap: StrokeCap.round,
              ),
            ),
          ),
        );
      } else if (snapshot.hasError) {
        return Center(
          child: Text(
            'Error: ${snapshot.error}',
            style: const TextStyle(fontSize: 18, color: Colors.red),
          ),
        );
      } else {
        return Scaffold(
          appBar: AppBar(
            centerTitle: true,
            title: Text(appWords[_appLanguage]!['contacts']!),
            backgroundColor: _headerColor,
            foregroundColor: Colors.white,
            actions: [
              IconButton(
                icon: const Icon(Icons.settings),
                onPressed: () async {
                  final result = await showSettingsDialog(
                    context,
                    _appLanguage,
                    _headerColor,
                  );
                  if (result != null) {
                    final language = result['language'] as String;
                    if (language != _appLanguage) {
                      safeSetState(() {
                        _appLanguage = language;
                      });
                    }

                    final color = result['color'] as Color;
                    if (color.r != _headerColor.r ||
                        color.g != _headerColor.g ||
                        color.b != _headerColor.b) {
                      setHeaderColor(color);
                    }
                  }
                },
                tooltip: 'Settings',
              ),
            ],
          ),
          body: RefreshIndicator(
            onRefresh: () async {
              refresh(Contact.getAllContacts(context));
            },
            child: Contact.count == 0
                ? Center(
                    child: Text(
                      appWords[_appLanguage]!['no_contacts']!,
                      style: TextStyle(fontSize: 18, color: Colors.grey),
                    ),
                  )
                : ListView.builder(
                    padding: const EdgeInsets.all(16),
                    itemCount: Contact.count,
                    itemBuilder: (context, index) {
                      final isExpanded = _expandedIndex == index;
                      return KeyedSubtree(
                        key: ValueKey('tile_${index}_$isExpanded'),
                        child: Contact.card(
                          context,
                          index,
                          isExpanded: isExpanded,
                          onExpansionChanged: (expanded) {
                            if (expanded) {
                              safeSetState(() {
                                _expandedIndex = index;
                              });
                            } else {
                              if (_expandedIndex == index) {
                                safeSetState(() {
                                  _expandedIndex = -1;
                                });
                              }
                            }
                          },
                        ),
                      );
                    },
                  ),
          ),
          floatingActionButton: FloatingActionButton(
            heroTag: null,
            onPressed: () async {
              final newContact = await showAddContactDialog(context);
              safeSetState(() {
                if (newContact != null) {
                  _loading = Contact.addContact(context, newContact);
                }
              });
            },
            shape: const CircleBorder(),
            backgroundColor: _headerColor.withValues(
              red: _headerColor.r * 0.8,
              green: _headerColor.g * 0.8,
              blue: _headerColor.b * 0.8,
            ),
            foregroundColor: Colors.white,
            child: const Icon(Icons.add),
          ),
          floatingActionButtonLocation:
              FloatingActionButtonLocation.centerDocked,
          extendBody: Contact.count != 0,
          bottomNavigationBar: BottomAppBar(color: Colors.transparent),
        );
      }
    },
  );
}
