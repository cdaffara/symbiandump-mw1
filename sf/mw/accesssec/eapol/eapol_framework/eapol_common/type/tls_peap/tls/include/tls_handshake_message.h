/*
* Copyright (c) 2001-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  EAP and WLAN authentication protocols.
*
*/

/*
* %version: 10.1.3 %
*/

#if !defined(_TLS_HANDSHAKE_MESSAGE_H_)
#define _TLS_HANDSHAKE_MESSAGE_H_

#include "eap_tools.h"
#include "eap_array.h"
#include "eap_variable_data.h"
#include "tls_handshake_header.h"
#include "tls_peap_types.h"
#include "abs_tls_message_hash.h"

#if defined(USE_EAP_TLS_SESSION_TICKET)
#include "tls_extension.h"
#include "eap_am_export.h"
#endif // #if defined(USE_EAP_TLS_SESSION_TICKET)

// Start: added by script change_export_macros.sh.
#if defined(EAP_NO_EXPORT_TLS_HANDSHAKE_MESSAGE_H)
	#define EAP_CLASS_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_NONSHARABLE 
	#define EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H 
	#define EAP_C_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H 
	#define EAP_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H 
	#define EAP_C_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H 
#elif defined(EAP_EXPORT_TLS_HANDSHAKE_MESSAGE_H)
	#define EAP_CLASS_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_EXPORT 
	#define EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_FUNC_EXPORT 
	#define EAP_C_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_C_FUNC_EXPORT 
	#define EAP_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H EAP_FUNC_EXPORT 
	#define EAP_C_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H EAP_C_FUNC_EXPORT 
#else
	#define EAP_CLASS_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_IMPORT 
	#define EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_FUNC_IMPORT 
	#define EAP_C_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_C_FUNC_IMPORT 
	#define EAP_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H 
	#define EAP_C_FUNC_EXPORT_TLS_HANDSHAKE_MESSAGE_H 
#endif
// End: added by script change_export_macros.sh.


/** @file */

//----------------------------------------------------------------------------

/// This class defines one TLS handshake message.
/**
 * This is class of the TLS-handshake message.
 * The parse_tls_protocol_handshake() function parses each TLS-handshake message
 * and generates this object. Parse and analyse of TLS-handshake is asyncronous.
 * m_is_analysed tells this message is analysed.
 * Analysed messages are skipped during the asyncronous
 * analyse of messages. Asyncronous analyse is needed
 * because of the PKI functions are asyncronous in
 * Symbian.
 */
class EAP_CLASS_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H tls_handshake_message_c
{
private:
	//--------------------------------------------------

	/// This is pointer to the tools class. @see abs_eap_am_tools_c.
	abs_eap_am_tools_c * const m_am_tools;

	/// This is pointer to interface of message HASH creation. See abs_tls_message_hash_c.
	abs_tls_message_hash_c * const m_message_hash;

	/// This buffer includes data of the handshake message.
	eap_variable_data_c m_tls_handshake_message_buffer;

	/// This The current time and date in standard UNIX 32-bit format (seconds
	/// since the midnight starting Jan 1, 1970, GMT) according to the sender's internal clock.
	eap_variable_data_c m_unix_time; 

	/// 28 bytes generated by a secure random number generator.
	eap_variable_data_c m_random_value;

	/// This is variable length. If not empty, the value identifies a session between the
	/// same client and server whose security parameters the client wishes to reuse.
	eap_variable_data_c m_session_id; 

	/// This is list of cipher suites.
	eap_array_c<u16_t> m_cipher_suites;

	/// This is list of compression methods.
	eap_array_c<u8_t> m_compression_methods;

#if defined(USE_EAP_TLS_SESSION_TICKET)
	/// This is list of TLS extensions.
	eap_array_c<tls_extension_c> m_tls_extensions;
#endif // #if defined(USE_EAP_TLS_SESSION_TICKET)

	/// This is certificate chain.
	eap_array_c<eap_variable_data_c> m_certificate_chain;

	/// This is list of certificaet authorities.
	eap_array_c<eap_variable_data_c> m_certificate_authorities;

	/// This is list of certificaet types.
	eap_array_c<u8_t> m_certificate_types;

	/// This is encrypted premaster secret.
	eap_variable_data_c m_encrypted_premaster_secret;

	/// This is ephemeral Diffie-Hellman public key.
	eap_variable_data_c m_public_dhe_key;

	/// This is ephemeral Diffie-Hellman prime.
	eap_variable_data_c m_dhe_prime;

	/// This is ephemeral Diffie-Hellman group generator.
	eap_variable_data_c m_dhe_group_generator;

	/// This is signed message hash.
	eap_variable_data_c m_signed_message_hash;

	/// This is finished data of TLS-Handshake/Finished message.
	eap_variable_data_c m_finished_data;

	/// This is the selected cipher suite.
	tls_cipher_suites_e m_selected_cipher_suite;

	/// This is the selected compression method.
	tls_compression_method_e m_selected_compression_method;

	/// This is handshake type. See tls_handshake_type_e.
	tls_handshake_type_e m_handshake_type;

	/// This flag tells this message is analysed.
	bool m_is_analysed;

	/// This indicates whether this object was generated successfully.
	bool m_is_valid;

	/// This indicates whether this object is client (true) or server (false).
	bool m_is_client;

	//--------------------------------------------------

	/**
	 * The set_is_valid() function sets the state of the object valid.
	 * The creator of this object calls this function after it is initialized. 
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H void set_is_valid();

	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H static eap_status_e u16_t_to_network_order(
		u16_t * const value,
		abs_eap_am_tools_c * const m_am_tools);

	//--------------------------------------------------
protected:
	//--------------------------------------------------

	//--------------------------------------------------
public:
	//--------------------------------------------------

	/**
	 * The destructor of the tls_handshake_message_c class does nothing special.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H virtual ~tls_handshake_message_c();

	/**
	 * The constructor of the tls_handshake_message_c class simply initializes the attributes.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H tls_handshake_message_c(
		abs_eap_am_tools_c * const tools,
		abs_tls_message_hash_c * const message_hash,
		const bool is_client);

	/**
	 * Object must indicate it's validity.
	 * If object initialization fails this function must return false.
	 * @return This function returns the validity of this object.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H bool get_is_valid();


	/**
	 * This function sets this message is analysed.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H void set_is_analysed();

	/**
	 * This function returns whether message is analysed (true) or not (false).
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H bool get_is_analysed();


	/**
	 * This function creates data of the Handshake message to internal buffer.
	 * Later this data is added to final TLS-record buffer.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e create_message_data();


	/**
	 * This function copies the appropriate fields of the TLS-handshake message.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_handshake_header_copy(const tls_handshake_header_c * const tls_handshake_header);


	/**
	 * This function sets the type of TLS-handshake message.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_handshake_type(tls_handshake_type_e type);

	/**
	 * This function returns the type of TLS-handshake message.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H tls_handshake_type_e get_handshake_type() const;


	/**
	 * This function copies the list of the cipher suites.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_cipher_suites(
		EAP_TEMPLATE_CONST eap_array_c<u16_t> * const cipher_suites);

	/**
	 * This function returns the list of the cipher suites.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<u16_t> * get_cipher_suites() EAP_TEMPLATE_CONST;


	/**
	 * This function copies the list of the compression methods.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_compression_methods(
		EAP_TEMPLATE_CONST eap_array_c<u8_t> * const compression_methods);

	/**
	 * This function returns the list of the compression methods.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<u8_t> * get_compression_methods() EAP_TEMPLATE_CONST;


#if defined(USE_EAP_TLS_SESSION_TICKET)
	/**
	 * This function copies the list of the TLS extensions.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_tls_extensions(
		EAP_TEMPLATE_CONST eap_array_c<tls_extension_c> * const compression_methods);
#endif // #if defined(USE_EAP_TLS_SESSION_TICKET)

#if defined(USE_EAP_TLS_SESSION_TICKET)
	/**
	 * This function returns the list of the TLS extensions.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<tls_extension_c> * get_tls_extensions() EAP_TEMPLATE_CONST;
#endif // #if defined(USE_EAP_TLS_SESSION_TICKET)


	/**
	 * This function copies the random value.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_random_value(
		const eap_variable_data_c * const random_value);

	/**
	 * This function returns the random value.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_random_value() const;


	/**
	 * This function copies the session id.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_session_id(
		const eap_variable_data_c * const session_id);

	/**
	 * This function returns the session id.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_session_id() const;


	/**
	 * This function copies the certificate chain.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_certificate_chain(
		EAP_TEMPLATE_CONST eap_array_c<eap_variable_data_c> * const certificate_chain);

	/**
	 * This function returns the certificate chain.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<eap_variable_data_c> * get_certificate_chain() EAP_TEMPLATE_CONST;


	/**
	 * This function copies the list of the certificate authorities.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_certificate_authorities(
		EAP_TEMPLATE_CONST eap_array_c<eap_variable_data_c> * const certificate_authorities);

	/**
	 * This function returns the list of the certificate authorities.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<eap_variable_data_c> * get_certificate_authorities() EAP_TEMPLATE_CONST;


	/**
	 * This function copies the list of the certificate types.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_certificate_types(
		EAP_TEMPLATE_CONST eap_array_c<u8_t> * const certificate_types);

	/**
	 * This function returns the list of the certificate types.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H EAP_TEMPLATE_CONST eap_array_c<u8_t> * get_certificate_types() EAP_TEMPLATE_CONST;


	/**
	 * This function sets the list of the selected cipher suite.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_selected_cipher_suite(const tls_cipher_suites_e selected_cipher_suite);

	/**
	 * This function returns the list of the selected cipher suite.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H tls_cipher_suites_e get_selected_cipher_suite() const;


	/**
	 * This function sets the list of the selected compression method.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_selected_compression_method(const tls_compression_method_e selected_compression_method);

	/**
	 * This function returns the list of the selected compression method.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H tls_compression_method_e get_selected_compression_method() const;


	/**
	 * This function copies the encrypted premaster secret.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_encrypted_premaster_secret(const eap_variable_data_c * const encrypted_premaster_secret);

	/**
	 * This function returns the encrypted premaster secret.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_encrypted_premaster_secret() const;


	/**
	 * This function copies the ephemeral Diffie-Hellman public key.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_public_dhe_key(const eap_variable_data_c * const public_dhe_key);

	/**
	 * This function returns the ephemeral Diffie-Hellman public key.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_public_dhe_key() const;


	/**
	 * This function copies the ephemeral Diffie-Hellman prime.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_dhe_prime(const eap_variable_data_c * const dhe_prime);

	/**
	 * This function returns the ephemeral Diffie-Hellman prime.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_dhe_prime() const;


	/**
	 * This function copies the ephemeral Diffie-Hellman group generator.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_dhe_group_generator(const eap_variable_data_c * const dhe_group_generator);

	/**
	 * This function returns the ephemeral Diffie-Hellman group generator.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_dhe_group_generator() const;


	/**
	 * This function copies the signed message HASH.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_signed_message_hash(const eap_variable_data_c * const signed_message_hash);

	/**
	 * This function returns the signed message HASH.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_signed_message_hash() const;


	/**
	 * This function copies the TLS-finished data.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e set_finished_data(const eap_variable_data_c * const finished_data);

	/**
	 * This function returns the TLS-finished data.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H const eap_variable_data_c * get_finished_data() const;


	/**
	 * This function adds data of the TLS-handshake message to tls_message_buffer.
	 */
	EAP_FUNC_VISIBILITY_TLS_HANDSHAKE_MESSAGE_H eap_status_e add_message_data(
		eap_variable_data_c * const tls_message_buffer);
	
	// 
	//--------------------------------------------------
}; // class tls_handshake_message_c


//--------------------------------------------------

#endif //#if !defined(_TLS_HANDSHAKE_MESSAGE_H_)



// End.